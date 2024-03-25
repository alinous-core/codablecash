/*
 * DownloadMempoolTransactionNodeCommand.cpp
 *
 *  Created on: 2023/11/03
 *      Author: iizuka
 */

#include "bc_p2p_cmd_node/SyncMempoolNodeCommand.h"
#include "bc_p2p_cmd_node/SyncMempoolNodeCommandResponse.h"

#include "bc/CodablecashNodeInstance.h"

#include "base/StackRelease.h"

#include "bc_memorypool/MemoryPool.h"

namespace codablecash {

SyncMempoolNodeCommand::SyncMempoolNodeCommand(const SyncMempoolNodeCommand &inst) : AbstractNodeCommand(inst) {
	this->trxType = inst.trxType;
	this->numRequestTrx = inst.numRequestTrx;
	this->offset = inst.offset;
}

SyncMempoolNodeCommand::SyncMempoolNodeCommand() : AbstractNodeCommand(TYPE_NODE_SYNC_MEMPOOL){
	this->trxType = TRX_BALANCE;
	this->numRequestTrx = 100;
	this->offset = 0;
}

SyncMempoolNodeCommand::~SyncMempoolNodeCommand() {

}

int SyncMempoolNodeCommand::binarySize() const {
	int total = AbstractNodeCommand::binarySize();

	total += sizeof(this->trxType);
	total += sizeof(this->numRequestTrx);
	total += sizeof(this->offset);

	return total;
}

void SyncMempoolNodeCommand::toBinary(ByteBuffer *buff) const {
	AbstractNodeCommand::toBinary(buff);

	buff->put(this->trxType);
	buff->putInt(this->numRequestTrx);
	buff->putInt(this->offset);
}

ByteBuffer* SyncMempoolNodeCommand::getSignBinary() const {
	int total = sizeof(this->trxType);
	total += sizeof(this->numRequestTrx);
	total += sizeof(this->offset);

	ByteBuffer* buff = ByteBuffer::allocateWithEndian(total, true);

	buff->put(this->trxType);
	buff->putInt(this->numRequestTrx);
	buff->putInt(this->offset);

	return buff;
}

void SyncMempoolNodeCommand::fromBinary(ByteBuffer *buff) {
	AbstractNodeCommand::fromBinary(buff);

	this->trxType = buff->get();
	this->numRequestTrx = buff->getInt();
	this->offset = buff->getInt();
}

IBlockObject* SyncMempoolNodeCommand::copyData() const noexcept {
	return new SyncMempoolNodeCommand(*this);
}

AbstractCommandResponse* SyncMempoolNodeCommand::executeAsNode(BlockchainNodeHandshake *nodeHandShake, CodablecashNodeInstance *inst, bool suspend) const {
	MemoryPool* memPool = inst->getMemoryPool();

	SyncMempoolNodeCommandResponse* res = new SyncMempoolNodeCommandResponse(); __STP(res);

	{
		MemPoolTransaction* memTrx = memPool->begin(); __STP(memTrx);
		if(this->trxType == TRX_CONTROL){
			memTrx->scanSyncControlTransaction(this->offset, this->numRequestTrx, res);
		}
		else if(this->trxType == TRX_ICC){
			memTrx->scanSyncInterChainCommunicationTransaction(this->offset, this->numRequestTrx, res);
		}
		else if(this->trxType == TRX_SMARTCONTRACT){
			memTrx->scanSyncSmartcontractTransaction(this->offset, this->numRequestTrx, res);
		}
		else{
			memTrx->scanSyncBalanceTransaction(this->offset, this->numRequestTrx, res);
		}
	}

	return __STP_MV(res);
}

void SyncMempoolNodeCommand::setTrxType(uint8_t trxType) noexcept {
	this->trxType = trxType;
}

void SyncMempoolNodeCommand::setNumRequestTrx(uint32_t numRequestTrx) noexcept {
	this->numRequestTrx = numRequestTrx;
}

void SyncMempoolNodeCommand::setOffset(uint32_t offset) noexcept {
	this->offset = offset;
}

} /* namespace codablecash */
