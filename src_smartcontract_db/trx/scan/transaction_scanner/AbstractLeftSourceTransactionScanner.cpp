/*
 * AbstractLeftSourceTransactionScanner.cpp
 *
 *  Created on: 2022/01/14
 *      Author: iizuka
 */

#include "trx/scan/transaction_scanner/AbstractLeftSourceTransactionScanner.h"

namespace codablecash {

AbstractLeftSourceTransactionScanner::AbstractLeftSourceTransactionScanner(const ScanResultMetadata* metadata, CdbTransaction* trx, const CdbTable* table, const AbstractScanCondition* filterCondition)
		: AbstractTransactionScanner(metadata, trx, table, filterCondition){

}

AbstractLeftSourceTransactionScanner::~AbstractLeftSourceTransactionScanner() {

}

void AbstractLeftSourceTransactionScanner::restartScan(VirtualMachine* vm) {
	AbstractTransactionScanner::restartScan(vm);
}

} /* namespace codablecash */
