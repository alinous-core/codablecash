/*
 * AbstractRightTransactionScanner.cpp
 *
 *  Created on: 2022/01/05
 *      Author: iizuka
 */

#include "trx/scan/transaction_scanner_join_right/AbstractRightTransactionScanner.h"

namespace codablecash {

AbstractRightTransactionScanner::AbstractRightTransactionScanner(ScanResultMetadata* metadata, CdbTransaction* trx,
		const CdbTable* table, const AbstractScanCondition* filterCondition)
			: AbstractTransactionScanner(metadata, trx, table, filterCondition){

}

AbstractRightTransactionScanner::~AbstractRightTransactionScanner() {

}

void AbstractRightTransactionScanner::restartScan(VirtualMachine* vm) {
}


} /* namespace codablecash */
