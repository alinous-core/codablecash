/*
 * AbstractTransactionLog.cpp
 *
 *  Created on: 2020/05/11
 *      Author: iizuka
 */

#include "trx/transaction_log/AbstractTransactionLog.h"
#include "trx/transaction_log/TransactionLogFactory.h"

#include "base_io/ByteBuffer.h"

#include "base/StackRelease.h"


namespace codablecash {

AbstractTransactionLog::AbstractTransactionLog(uint8_t type, uint64_t oid) {
	this->type = type;
	this->oid = oid;
}

AbstractTransactionLog::~AbstractTransactionLog() {
}

AbstractTransactionLog* AbstractTransactionLog::copy() const {
	int size = binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(size, true); __STP(buff);

	toBinary(buff);

	buff->position(0);
	AbstractTransactionLog* log = TransactionLogFactory::createFromBinary(buff);

	return log;
}


} /* namespace codablecash */
