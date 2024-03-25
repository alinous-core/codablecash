/*
 * RightTableIndexTransactionScanner.h
 *
 *  Created on: 2020/12/20
 *      Author: iizuka
 */

#ifndef TRX_SCAN_TRANSACTION_SCANNER_JOIN_RIGHT_RIGHTTABLEINDEXTRANSACTIONSCANNER_H_
#define TRX_SCAN_TRANSACTION_SCANNER_JOIN_RIGHT_RIGHTTABLEINDEXTRANSACTIONSCANNER_H_

#include "trx/scan/transaction_scanner_join_right/AbstractRightTransactionScanner.h"

namespace codablecash {

class CdbTableIndex;
class TableStore;
class IndexStore;
class IndexScanner;
class CdbRecord;
class UpdateCacheIndexScanner;


class RightTableIndexTransactionScanner : public AbstractRightTransactionScanner {
public:
	RightTableIndexTransactionScanner(ScanResultMetadata* metadata, CdbTransaction* trx,
			const CdbTable* table, const AbstractScanCondition* filterCondition, CdbTableIndex* index);
	virtual ~RightTableIndexTransactionScanner();

	virtual void start(VirtualMachine* vm);
	virtual void reset(const AbstractCdbKey* key);

	virtual bool hasNext(VirtualMachine* vm);
	virtual const CdbRecord* next(VirtualMachine* vm);

	virtual void shutdown();

private:
	void setScanner(IndexScanner* scanner) noexcept;

private:
	CdbTableIndex* index;

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

#endif /* TRX_SCAN_TRANSACTION_SCANNER_JOIN_RIGHT_RIGHTTABLEINDEXTRANSACTIONSCANNER_H_ */
