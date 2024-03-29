/*
 * AbstractDdlLog.h
 *
 *  Created on: 2020/05/12
 *      Author: iizuka
 */

#ifndef TRANSACTION_LOG_ABSTRACTDDLLOG_H_
#define TRANSACTION_LOG_ABSTRACTDDLLOG_H_

#include "trx/transaction_log/AbstractTransactionLog.h"

namespace codablecash {

class AbstractDdlLog : public AbstractTransactionLog {
public:
	AbstractDdlLog(uint8_t type, uint64_t oid);
	virtual ~AbstractDdlLog();
};

} /* namespace codablecash */

#endif /* TRANSACTION_LOG_ABSTRACTDDLLOG_H_ */
