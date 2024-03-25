/*
 * TableTransactionOrIndexScanner.h
 *
 *  Created on: 2020/12/11
 *      Author: iizuka
 */

#ifndef TRX_SCAN_TRANSACTION_SCANNER_TABLETRANSACTIONORINDEXSCANNER_H_
#define TRX_SCAN_TRANSACTION_SCANNER_TABLETRANSACTIONORINDEXSCANNER_H_

#include "trx/scan/transaction_scanner/AbstractLeftSourceTransactionScanner.h"

#include "base/ArrayList.h"

namespace codablecash {

class CdbTransaction;
class TableStore;
class OrIndexAccessCollection;
class IJoinLeftSource;
class RecordScanIdentityCache;

class TableTransactionOrIndexScanner : public AbstractLeftSourceTransactionScanner {
public:
	TableTransactionOrIndexScanner(ScanResultMetadata* metadata, CdbTransaction* trx, TableStore* tableStore
			, const AbstractScanCondition* filterCondition, const OrIndexAccessCollection* index);
	virtual ~TableTransactionOrIndexScanner();

	virtual void start(VirtualMachine* vm);
	virtual void restartScan(VirtualMachine* vm);
	virtual bool hasNext(VirtualMachine* vm);
	virtual const CdbRecord* next(VirtualMachine* vm);

	virtual void shutdown();

	void addLeftSource(IJoinLeftSource* source) noexcept;

private:
	void setRecord(CdbRecord* rec) noexcept;

private:
	const OrIndexAccessCollection* index;

	ArrayList<IJoinLeftSource> list;
	int pos;

	RecordScanIdentityCache* cache;
	CdbRecord* nextRecord;
};

} /* namespace codablecash */

#endif /* TRX_SCAN_TRANSACTION_SCANNER_TABLETRANSACTIONORINDEXSCANNER_H_ */
