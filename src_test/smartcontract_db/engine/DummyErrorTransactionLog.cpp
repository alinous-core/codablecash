/*
 * DummyErrorTransactionLog.cpp
 *
 *  Created on: 2020/05/16
 *      Author: iizuka
 */

#include "DummyErrorTransactionLog.h"

#include "trx/transaction_log/AbstractTransactionLog.h"

#include "engine/CdbException.h"


namespace codablecash {

DummyErrorTransactionLog::DummyErrorTransactionLog(uint64_t oid) : CreateTableLog(oid) {

}

DummyErrorTransactionLog::~DummyErrorTransactionLog() {

}

void DummyErrorTransactionLog::commit(CdbTransactionManager* trxManager, CdbTransaction* trx) {
	throw new CdbException(__FILE__, __LINE__);
}

} /* namespace codablecash */
