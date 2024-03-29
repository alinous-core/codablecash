/*
 * DummyErrorTransactionLog.h
 *
 *  Created on: 2020/05/16
 *      Author: iizuka
 */

#ifndef SMARTCONTRACT_DB_ENGINE_DUMMYERRORTRANSACTIONLOG_H_
#define SMARTCONTRACT_DB_ENGINE_DUMMYERRORTRANSACTIONLOG_H_

#include "trx/transaction_log/CreateTableLog.h"

namespace codablecash {

class DummyErrorTransactionLog : public CreateTableLog {
public:
	explicit DummyErrorTransactionLog(uint64_t oid);
	virtual ~DummyErrorTransactionLog();

	virtual void commit(CdbTransactionManager* trxManager, CdbTransaction* trx);
};

} /* namespace codablecash */

#endif /* SMARTCONTRACT_DB_ENGINE_DUMMYERRORTRANSACTIONLOG_H_ */
