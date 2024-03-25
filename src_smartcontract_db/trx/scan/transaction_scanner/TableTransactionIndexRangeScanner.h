/*
 * TableTransactionIndexRangeScanner.h
 *
 *  Created on: 2021/01/06
 *      Author: iizuka
 */

#ifndef TRX_SCAN_TRANSACTION_SCANNER_TABLETRANSACTIONINDEXRANGESCANNER_H_
#define TRX_SCAN_TRANSACTION_SCANNER_TABLETRANSACTIONINDEXRANGESCANNER_H_

#include "trx/scan/transaction_scanner/AbstractLeftSourceTransactionScanner.h"

namespace codablecash {

class CdbTransaction;
class TableStore;
class IndexStore;
class IndexScanner;
class CdbRecord;
class AbstractColumnsIndexAccess;
class UpdateCacheIndexScanner;

class TableTransactionIndexRangeScanner : public AbstractLeftSourceTransactionScanner {
public:
	TableTransactionIndexRangeScanner(ScanResultMetadata* metadata, CdbTransaction* trx, TableStore* tableStore
			, const AbstractScanCondition* filterCondition, AbstractColumnsIndexAccess* indexCandidate);
	virtual ~TableTransactionIndexRangeScanner();

	virtual void start(VirtualMachine* vm);
	virtual void restartScan(VirtualMachine* vm);
	virtual bool hasNext(VirtualMachine* vm);
	virtual const CdbRecord* next(VirtualMachine* vm);

	virtual void shutdown();

private:
	void setScanner(IndexScanner* scanner) noexcept;

private:
	AbstractColumnsIndexAccess* indexCandidate;

	TableStore* tableStore;
	IndexStore* indexStore;
	IndexScanner* scanner;
	bool scanedScanner;

	UpdateCacheIndexScanner* updatedRecordsScanner;
	UpdateCacheIndexScanner* insertedRecordsScanner;
	bool scannedInsertedScanner;

	CdbRecord* record;
};

} /* namespace codablecash */

#endif /* TRX_SCAN_TRANSACTION_SCANNER_TABLETRANSACTIONINDEXRANGESCANNER_H_ */
