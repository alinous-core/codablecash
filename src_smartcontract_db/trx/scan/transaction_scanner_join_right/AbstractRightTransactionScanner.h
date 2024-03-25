/*
 * AbstractRightTransactionScanner.h
 *
 *  Created on: 2022/01/05
 *      Author: iizuka
 */

#ifndef TRX_SCAN_TRANSACTION_SCANNER_JOIN_RIGHT_ABSTRACTRIGHTTRANSACTIONSCANNER_H_
#define TRX_SCAN_TRANSACTION_SCANNER_JOIN_RIGHT_ABSTRACTRIGHTTRANSACTIONSCANNER_H_

#include "trx/scan/transaction_scanner_join/IJoinRightSource.h"

#include "trx/scan/transaction_scanner/AbstractTransactionScanner.h"

namespace codablecash {

class AbstractRightTransactionScanner : public IJoinRightSource, public AbstractTransactionScanner {
public:
	AbstractRightTransactionScanner(ScanResultMetadata* metadata, CdbTransaction* trx,
			const CdbTable* table, const AbstractScanCondition* filterCondition);
	virtual ~AbstractRightTransactionScanner();

	virtual void restartScan(VirtualMachine* vm);

	virtual const ScanResultMetadata* getMetadata() const noexcept {
		return this->metadata;
	}
};

} /* namespace codablecash */

#endif /* TRX_SCAN_TRANSACTION_SCANNER_JOIN_RIGHT_ABSTRACTRIGHTTRANSACTIONSCANNER_H_ */
