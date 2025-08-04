/*
 * ClientSyncHeaderNodeCommand.cpp
 *
 *  Created on: Apr 22, 2025
 *      Author: iizuka
 */

#include "bc_p2p_cmd_client/ClientSyncHeaderCommand.h"
#include "bc_p2p_cmd_client/ClientSyncHeaderHeightData.h"
#include "bc_p2p_cmd_client/ClientBlockHeaderTransferData.h"
#include "bc_p2p_cmd_client/ClientSyncHeaderCommandResponse.h"

#include "bc/CodablecashNodeInstance.h"
#include "bc/ExceptionThrower.h"

#include "bc_p2p/BlockchainZoneException.h"
#include "bc_p2p/ClientNodeHandshake.h"

#include "base/StackRelease.h"

#include "bc_block/Block.h"

#include "bc_block_body/BlockBody.h"

#include "bc_wallet_filter/BloomFilter1024.h"


namespace codablecash {

ClientSyncHeaderCommand::ClientSyncHeaderCommand(const ClientSyncHeaderCommand &inst)
		: AbstractClientRequestCommand(inst) {
	this->zone = inst.zone;
	this->numRequestHeight = inst.numRequestHeight;
	this->startHeight = inst.startHeight;
}

ClientSyncHeaderCommand::ClientSyncHeaderCommand() : AbstractClientRequestCommand(TYPE_CLIENT_SYNC_HEADER) {
	this->zone = 0;
	this->numRequestHeight = 100;
	this->startHeight = 0;
}

ClientSyncHeaderCommand::~ClientSyncHeaderCommand() {

}

int ClientSyncHeaderCommand::binarySize() const {
	int total = AbstractClientRequestCommand::binarySize();

	total += sizeof(this->zone);
	total += sizeof(this->numRequestHeight);
	total += sizeof(this->startHeight);

	return total;
}

void ClientSyncHeaderCommand::toBinary(ByteBuffer *buff) const {
	AbstractClientRequestCommand::toBinary(buff);

	buff->putShort(this->zone);
	buff->putInt(this->numRequestHeight);
	buff->putLong(this->startHeight);
}

void ClientSyncHeaderCommand::fromBinary(ByteBuffer *buff) {
	AbstractClientRequestCommand::fromBinary(buff);

	this->zone = buff->getShort();
	this->numRequestHeight = buff->getInt();
	this->startHeight = buff->getLong();
}

ByteBuffer* ClientSyncHeaderCommand::getSignBinary() const {
	int total = sizeof(this->zone);
	total += sizeof(this->numRequestHeight);
	total += sizeof(this->startHeight);

	ByteBuffer* buff = ByteBuffer::allocateWithEndian(total, true);

	buff->putShort(this->zone);
	buff->putInt(this->numRequestHeight);
	buff->putLong(this->startHeight);

	return buff;
}

IBlockObject* ClientSyncHeaderCommand::copyData() const noexcept {
	return new ClientSyncHeaderCommand(*this);
}

void ClientSyncHeaderCommand::setZone(uint16_t zone) noexcept {
	this->zone = zone;
}

void ClientSyncHeaderCommand::setNumRequestHeight(uint32_t numRequestHeight) noexcept {
	this->numRequestHeight = numRequestHeight;
}

void ClientSyncHeaderCommand::setStartHeight(uint32_t offset) noexcept {
	this->startHeight = offset;
}

AbstractCommandResponse* ClientSyncHeaderCommand::executeAsClient(ClientNodeHandshake *clientHandshake, CodablecashNodeInstance *inst) const {
	uint16_t zoneSelf = inst->getZoneSelf();
	ExceptionThrower<BlockchainZoneException>::throwExceptionIfCondition(zoneSelf != this->zone, L"Wrong zone.", __FILE__, __LINE__);

	ClientSyncHeaderCommandResponse* respose = new ClientSyncHeaderCommandResponse(); __STP(respose);

	{
		BlockchainController* ctrl = inst->getController();

		ctrl->getSyncHeaderData(zoneSelf, this->startHeight, this->numRequestHeight, respose);
	}

	// transactions
	const ArrayList<BloomFilter1024>* filtersList = clientHandshake->getBloomFilters();
	if(filtersList != nullptr){
		int maxLoop = respose->getDataCount();
		for(int i = 0; i != maxLoop; ++i){
			const ClientSyncHeaderHeightData* data = respose->get(i);

			handleBlockHeightTransactions(data, filtersList, inst);
		}
	}

	return __STP_MV(respose);
}

void ClientSyncHeaderCommand::handleBlockHeightTransactions(const ClientSyncHeaderHeightData *data, const ArrayList<BloomFilter1024> *filtersList,
		CodablecashNodeInstance *inst) const {
	ArrayList<ClientBlockHeaderTransferData>* list = data->getDataList();

	BlockchainController* ctrl = inst->getController();

	int maxLoop = list->size();
	for(int i = 0; i != maxLoop; ++i){
		ClientBlockHeaderTransferData* data = list->get(i);

		const BlockHeader* header = data->getHeader();
		uint16_t zone = header->getZone();
		uint64_t height = header->getHeight();
		const BlockHeaderId* headerId = header->getId();

		Block* block = ctrl->getBlocksHeightAt(zone, height, headerId); __STP(block);

		// server side trx fetch
		handleBlockTransactions(data, block, filtersList, inst);
	}
}

void ClientSyncHeaderCommand::handleBlockTransactions(ClientBlockHeaderTransferData *data, const Block *block, const ArrayList<BloomFilter1024> *filtersList, CodablecashNodeInstance *inst) const {
	BlockBody* body = block->getBody();

	data->setFiltersList(filtersList);
	body->visitTransactions(data, block);
}

} /* namespace codablecash */
