/*
 * AbstractTransactionAcceptionQueueCommand.cpp
 *
 *  Created on: 2023/10/02
 *      Author: iizuka
 */

#include "command_queue_cmd/AbstractTransactionAcceptionQueueCommand.h"

#include "bc_base/BinaryUtils.h"

#include "data_history_data/TransactionTransferData.h"

#include "base/StackRelease.h"

#include "pubsub/IPubsubCommandListner.h"

#include "bc/CodablecashNodeInstance.h"
#include "bc/ISystemLogger.h"

#include "bc_trx/AbstractBlockchainTransaction.h"

#include "bc_memorypool/MemoryPool.h"
#include "bc_memorypool/MemPoolTransaction.h"


namespace codablecash {

AbstractTransactionAcceptionQueueCommand::AbstractTransactionAcceptionQueueCommand(const AbstractTransactionAcceptionQueueCommand &inst)
		: AbstructNodeQueueCommand(inst) {
	this->data = inst.data != nullptr ? dynamic_cast<TransactionTransferData*>(inst.data->copyData()) : nullptr;
}

AbstractTransactionAcceptionQueueCommand::AbstractTransactionAcceptionQueueCommand(uint32_t type)
	: AbstructNodeQueueCommand(type) {
	this->data = nullptr;
}

AbstractTransactionAcceptionQueueCommand::~AbstractTransactionAcceptionQueueCommand() {
	delete this->data;
}

int AbstractTransactionAcceptionQueueCommand::binarySize() const {
	BinaryUtils::checkNotNull(this->data);

	int total = sizeof(this->type);
	total += this->data->binarySize();

	return total;
}

void AbstractTransactionAcceptionQueueCommand::toBinary(ByteBuffer *buff) const {
	BinaryUtils::checkNotNull(this->data);

	buff->putInt(this->type);
	this->data->toBinary(buff);
}

void AbstractTransactionAcceptionQueueCommand::fromBinary(ByteBuffer *buff) {
	AbstractTransferedData* d = TransactionTransferData::createFromBinary(buff); __STP(d);
	this->data = dynamic_cast<TransactionTransferData*>(d);

	BinaryUtils::checkNotNull(this->data);
	__STP_MV(d);
}

void AbstractTransactionAcceptionQueueCommand::setData(const TransactionTransferData *data) noexcept {
	delete this->data;
	this->data = dynamic_cast<TransactionTransferData*>(data->copyData());
}

void AbstractTransactionAcceptionQueueCommand::execute(const PubSubId *pubsubId, IPubsubCommandListner *listner) {
	IPubsubCommandExecutor* exec = listner->getExecutor();
	CodablecashNodeInstance* inst = dynamic_cast<CodablecashNodeInstance*>(exec);

	ISystemLogger* logger = inst->getLogger();

	AbstractBlockchainTransaction* trx = this->data->getTransaction();

	MemoryPool* memPool = inst->getMemoryPool();
	MemPoolTransaction* memTrx = memPool->begin(); __STP(memTrx);

	const TransactionId* trxId = trx->getTransactionId();
	AbstractBlockchainTransaction* mtrx = memPool->__getTransactionById(trxId); __STP(mtrx);

	if(mtrx == nullptr){
		memPool->__putTransaction(trx);

		UnicodeString msg(L"put mempool : ");
		UnicodeString* trxIdStr = trxId->toString(); __STP(trxIdStr);
		msg.append(trxIdStr);

		logger->debugLog(ISystemLogger::DEBUG_TRX_MEMPOOL_REGISTER, &msg, __FILE__, __LINE__);
	}
}

} /* namespace codablecash */
