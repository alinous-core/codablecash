/*
 * TableTransactionScan.h
 *
 *  Created on: 2020/06/10
 *      Author: iizuka
 */

#ifndef TRANSACTION_SCANNER_TABLETRANSACTIONSCANNER_H_
#define TRANSACTION_SCANNER_TABLETRANSACTIONSCANNER_H_

#include "trx/scan/transaction_scanner/AbstractLeftSourceTransactionScanner.h"

namespace codablecash {

class TableStore;
class RecordScanner;
class AbstractLockHandle;

class TableTransactionScanner : public AbstractLeftSourceTransactionScanner {
public:
	TableTransactionScanner(const ScanResultMetadata* metadata, CdbTransaction* trx, TableStore* tableStore, const AbstractScanCondition* filterCondition);
	virtual ~TableTransactionScanner();

	virtual void start(VirtualMachine* vm);
	virtual void restartScan(VirtualMachine* vm);
	virtual bool hasNext(VirtualMachine* vm);
	virtual const CdbRecord* next(VirtualMachine* vm);

	virtual void shutdown();

private:
	TableStore* tableStore;
	RecordScanner* internalScanner;

	CdbRecord* nextRecord;
	bool scanedStore;

	AbstractLockHandle* lockHandle;
};

} /* namespace codablecash */

#endif /* TRANSACTION_SCANNER_TABLETRANSACTIONSCANNER_H_ */
