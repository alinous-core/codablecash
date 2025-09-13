/*
 * ClientFetchMempoolTrx.cpp
 *
 *  Created on: Aug 15, 2025
 *      Author: iizuka
 */

#include "base/StackRelease.h"

#include "bc_p2p/ClientNodeHandshake.h"

#include "bc_p2p_cmd_client/ClientFetchMempoolTrxCommand.h"
#include "bc_p2p_cmd_client/ClientFetchMempoolTrxCommandResponse.h"

#include "bc/CodablecashNodeInstance.h"

#include "bc_status_cache/BlockchainController.h"

#include "bc_trx/AbstractBlockchainTransaction.h"


namespace codablecash {

ClientFetchMempoolTrxCommand::ClientFetchMempoolTrxCommand(const ClientFetchMempoolTrxCommand &inst) : AbstractClientRequestCommand(inst) {
	this->zone = inst.zone;
}

ClientFetchMempoolTrxCommand::ClientFetchMempoolTrxCommand() : AbstractClientRequestCommand(TYPE_CLIENT_FETCH_MEMPOOL_TRX) {
	this->zone = 0;
}

ClientFetchMempoolTrxCommand::~ClientFetchMempoolTrxCommand() {

}
int ClientFetchMempoolTrxCommand::binarySize() const {
	int total = AbstractClientRequestCommand::binarySize();

	total += sizeof(this->zone);

	return total;
}

void ClientFetchMempoolTrxCommand::toBinary(ByteBuffer *buff) const {
	AbstractClientRequestCommand::toBinary(buff);

	buff->putShort(this->zone);
}

void ClientFetchMempoolTrxCommand::fromBinary(ByteBuffer *buff) {
	AbstractClientRequestCommand::fromBinary(buff);

	this->zone = buff->getShort();
}


ByteBuffer* ClientFetchMempoolTrxCommand::getSignBinary() const {
	int total = sizeof(this->zone);

	ByteBuffer* buff = ByteBuffer::allocateWithEndian(total, true); __STP(buff);
	buff->putShort(this->zone);

	return __STP_MV(buff);
}

IBlockObject* ClientFetchMempoolTrxCommand::copyData() const noexcept {
	return new ClientFetchMempoolTrxCommand(*this);
}

void ClientFetchMempoolTrxCommand::setZone(uint16_t zone) noexcept {
	this->zone = zone;
}

AbstractCommandResponse* ClientFetchMempoolTrxCommand::executeAsClient(ClientNodeHandshake *clientHandshake, CodablecashNodeInstance *inst) const {
	BlockchainController* ctrl = inst->getController();

	const ArrayList<BloomFilter1024>* filters = clientHandshake->getBloomFilters();

	ArrayList<AbstractBlockchainTransaction>* reslist = ctrl->fetchMempoolTrx(filters); __STP(reslist);
	reslist->setDeleteOnExit();

	ClientFetchMempoolTrxCommandResponse* res = new ClientFetchMempoolTrxCommandResponse(); __STP(res);

	int maxLoop = reslist->size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractBlockchainTransaction* trx = reslist->get(i);

		res->addTransaction(trx);
	}

	return __STP_MV(res);
}

} /* namespace codablecash */
