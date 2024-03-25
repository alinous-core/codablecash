/*
 * AbstractTransactionScanner.h
 *
 *  Created on: 2020/06/10
 *      Author: iizuka
 */

#ifndef TRANSACTION_SCANNER_ABSTRACTTRANSACTIONSCANNER_H_
#define TRANSACTION_SCANNER_ABSTRACTTRANSACTIONSCANNER_H_

namespace alinous {
class VirtualMachine;
}
using namespace alinous;

namespace codablecash {

class CdbTransaction;
class CdbRecord;
class CdbTable;
class InsertRecordsCacheCursor;
class UpdatedRecordCursor;
class ScanResultMetadata;
class AbstractScanCondition;
class TransactionUpdateCache;

class AbstractTransactionScanner {
public:
	AbstractTransactionScanner(const ScanResultMetadata* metadata, CdbTransaction* trx, const CdbTable* table, const AbstractScanCondition* filterCondition);
	virtual ~AbstractTransactionScanner();

	virtual void start(VirtualMachine* vm) = 0;
	virtual bool hasNext(VirtualMachine* vm) = 0;
	virtual const CdbRecord* next(VirtualMachine* vm) = 0;

	virtual void shutdown() = 0;

	virtual void restartScan(VirtualMachine* vm);

	bool hasInsertedRecord() const noexcept;
	const CdbRecord* nextInsertedRecord() noexcept;

	bool hasUpdatedRecord() const noexcept;
	const CdbRecord* nextUpdatedRecord() noexcept;


	bool checkUpdated(const CdbRecord* record) const noexcept;

protected:
	ScanResultMetadata* metadata;
	CdbTransaction* trx;
	TransactionUpdateCache* trxCache;
	const CdbTable* table;
	AbstractScanCondition* filterCondition;

	InsertRecordsCacheCursor* insertedCacheCursor;
	UpdatedRecordCursor* updatedCacheCurser;

	mutable bool scanedInserted;
	mutable bool scanedUpdated;
};

} /* namespace codablecash */

#endif /* TRANSACTION_SCANNER_ABSTRACTTRANSACTIONSCANNER_H_ */
