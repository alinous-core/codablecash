/*
 * AbstractLeftSourceTransactionScanner.h
 *
 *  Created on: 2022/01/14
 *      Author: iizuka
 */

#ifndef TRX_SCAN_TRANSACTION_SCANNER_ABSTRACTLEFTSOURCETRANSACTIONSCANNER_H_
#define TRX_SCAN_TRANSACTION_SCANNER_ABSTRACTLEFTSOURCETRANSACTIONSCANNER_H_

#include "trx/scan/transaction_scanner/AbstractTransactionScanner.h"

#include "trx/scan/transaction_scanner_join/IJoinLeftSource.h"

namespace codablecash {

class AbstractLeftSourceTransactionScanner : public AbstractTransactionScanner, public IJoinLeftSource {
public:
	AbstractLeftSourceTransactionScanner(const ScanResultMetadata* metadata, CdbTransaction* trx, const CdbTable* table, const AbstractScanCondition* filterCondition);
	virtual ~AbstractLeftSourceTransactionScanner();

	virtual void restartScan(VirtualMachine* vm);

	virtual const ScanResultMetadata* getMetadata() const noexcept {
		return this->metadata;
	}

};

} /* namespace codablecash */

#endif /* TRX_SCAN_TRANSACTION_SCANNER_ABSTRACTLEFTSOURCETRANSACTIONSCANNER_H_ */
