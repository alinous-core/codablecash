/*
 * AbstractDdlLog.cpp
 *
 *  Created on: 2020/05/12
 *      Author: iizuka
 */

#include "trx/transaction_log/AbstractDdlLog.h"

namespace codablecash {

AbstractDdlLog::AbstractDdlLog(uint8_t type, uint64_t oid) : AbstractTransactionLog(type, oid) {

}

AbstractDdlLog::~AbstractDdlLog() {

}

} /* namespace codablecash */
