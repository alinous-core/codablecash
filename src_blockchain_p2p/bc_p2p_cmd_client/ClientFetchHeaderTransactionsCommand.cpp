/*
 * ClientFetchHeaderTransactionsCommand.cpp
 *
 *  Created on: Aug 20, 2025
 *      Author: iizuka
 */

#include "bc_p2p_cmd_client/ClientFetchHeaderTransactionsCommand.h"
#include "bc_p2p_cmd_client/ClientBlockHeaderTransferData.h"
#include "bc_p2p_cmd_client/ClientFetchHeaderTransactionsCommandResponse.h"

#include "bc_block/BlockHeaderId.h"
#include "bc_block/Block.h"
#include "bc_block_body/BlockBody.h"

#include "base/StackRelease.h"

#include "bc_base/BinaryUtils.h"

#include "bc/CodablecashNodeInstance.h"
#include "bc/ExceptionThrower.h"

#include "bc_p2p/ClientNodeHandshake.h"
#include "bc_p2p/BlockchainZoneException.h"

#include "bc_status_cache/BlockchainController.h"

#include "bc_block/BlockHeader.h"


namespace codablecash {

ClientFetchHeaderTransactionsCommand::ClientFetchHeaderTransactionsCommand(const ClientFetchHeaderTransactionsCommand &inst) : AbstractClientRequestCommand(inst) {
	this->headerId = inst.headerId != nullptr ? dynamic_cast<BlockHeaderId*>(inst.headerId->copyData()) : nullptr;
	this->height = inst.height;
	this->zone = inst.zone;
}

ClientFetchHeaderTransactionsCommand::ClientFetchHeaderTransactionsCommand() : AbstractClientRequestCommand(TYPE_CLIENT_FETCH_HEADER_TRX) {
	this->headerId = nullptr;
	this->height = 0;
	this->zone = 0;
}

ClientFetchHeaderTransactionsCommand::~ClientFetchHeaderTransactionsCommand() {
	delete this->headerId;
}

int ClientFetchHeaderTransactionsCommand::binarySize() const {
	BinaryUtils::checkNotNull(this->headerId);

	int total = AbstractClientRequestCommand::binarySize();

	total += sizeof(this->zone);
	total += sizeof(this->height);
	total += this->headerId->binarySize();

	return total;
}

void ClientFetchHeaderTransactionsCommand::toBinary(ByteBuffer *buff) const {
	BinaryUtils::checkNotNull(this->headerId);

	AbstractClientRequestCommand::toBinary(buff);

	buff->putShort(this->zone);
	buff->putLong(this->height);
	this->headerId->toBinary(buff);
}

void ClientFetchHeaderTransactionsCommand::fromBinary(ByteBuffer *buff) {
	AbstractClientRequestCommand::fromBinary(buff);

	this->zone = buff->getShort();
	this->height = buff->getLong();
	this->headerId = BlockHeaderId::fromBinary(buff);
}

ByteBuffer* ClientFetchHeaderTransactionsCommand::getSignBinary() const {
	int total = sizeof(this->zone);
	total += sizeof(this->height);
	total += this->headerId->binarySize();

	ByteBuffer* buff = ByteBuffer::allocateWithEndian(total, true); __STP(buff);
	buff->putShort(this->zone);
	buff->putLong(this->height);
	this->headerId->toBinary(buff);

	return __STP_MV(buff);
}

IBlockObject* ClientFetchHeaderTransactionsCommand::copyData() const noexcept {
	return new ClientFetchHeaderTransactionsCommand(*this);
}

void ClientFetchHeaderTransactionsCommand::setBlockHeaderId(const BlockHeaderId *headerId) {
	delete this->headerId;
	this->headerId = dynamic_cast<BlockHeaderId*>(headerId->copyData());
}

AbstractCommandResponse* ClientFetchHeaderTransactionsCommand::executeAsClient(ClientNodeHandshake *clientHandshake, CodablecashNodeInstance *inst) const {
	uint16_t zoneSelf = inst->getZoneSelf();
	ExceptionThrower<BlockchainZoneException>::throwExceptionIfCondition(zoneSelf != this->zone, L"Wrong zone.", __FILE__, __LINE__);


	ClientFetchHeaderTransactionsCommandResponse* response = new ClientFetchHeaderTransactionsCommandResponse(); __STP(response);

	const ArrayList<BloomFilter1024>* filters = clientHandshake->getBloomFilters();

	{
		BlockchainController* ctrl = inst->getController();

		Block* block = ctrl->getBlockHeightAt(this->zone, this->height, this->headerId); __STP(block);
		// if block exists
		if(block != nullptr){
			BlockBody* body = block->getBody();
			BlockHeader* header = block->getHeader();

			ClientBlockHeaderTransferData* transferData = new ClientBlockHeaderTransferData(); __STP(transferData);
			transferData->setHeader(header);
			transferData->setFiltersList(filters);

			// setup voter certificats
			header->buildVoteTransactionIdCertificate(block, transferData);

			// import transactions
			body->visitTransactions(transferData, block);

			response->setTransferData(__STP_MV(transferData));
		}
	}

	return __STP_MV(response);
}

} /* namespace codablecash */
