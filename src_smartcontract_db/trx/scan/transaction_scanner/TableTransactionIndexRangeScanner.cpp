/*
 * TableTransactionIndexRangeScanner.cpp
 *
 *  Created on: 2021/01/06
 *      Author: iizuka
 */

#include "trx/scan/transaction_scanner/TableTransactionIndexRangeScanner.h"

#include "scan_select/scan_condition/base/AbstractScanCondition.h"

#include "trx/transaction/CdbTransaction.h"

#include "schema_table/table_store/TableStore.h"
#include "schema_table/table_store/IndexStore.h"

#include "schema_table/record/table_record/CdbRecord.h"

#include "schema_table/record/table_record_key/CdbRecordKey.h"

#include "trx/scan/transaction_scan_result/ScanResultMetadata.h"

#include "scan_select/scan/IndexScanner.h"

#include "trx/transaction/CdbTransactionManager.h"

#include "engine/CdbOid.h"

#include "schema_table/table/CdbTable.h"
#include "schema_table/table/CdbTableIndex.h"

#include "schema_table/table_store/CdbStorageManager.h"

#include "scan_select/scan_condition/base/AbstractScanConditionElement.h"

#include "base/StackRelease.h"

#include "schema_table/record/table_record_value/AbstractCdbValue.h"

#include "btree/AbstractBtreeKey.h"
#include "scan_select/scan_planner/scanner/index_resolv/AbstractColumnsIndexAccess.h"

#include "trx/scan/transaction_update_cache/TransactionUpdateCache.h"

#include "trx/scan/transaction_update_cache_index/UpdateCacheIndexScanner.h"
namespace codablecash {

TableTransactionIndexRangeScanner::TableTransactionIndexRangeScanner(ScanResultMetadata* metadata,
		CdbTransaction* trx, TableStore* tableStore, const AbstractScanCondition* filterCondition, AbstractColumnsIndexAccess* indexCandidate)
		: AbstractLeftSourceTransactionScanner(metadata, trx, tableStore->getTable(), filterCondition){
	this->indexCandidate = indexCandidate->clone();
	this->tableStore = nullptr;
	this->indexStore = nullptr;
	this->scanner = nullptr;
	this->record = nullptr;
	this->scanedScanner = false;

	this->insertedRecordsScanner = nullptr;
	this->scannedInsertedScanner = false;
	this->updatedRecordsScanner = nullptr;
}

TableTransactionIndexRangeScanner::~TableTransactionIndexRangeScanner() {
	shutdown();

	delete this->indexCandidate;

	delete this->updatedRecordsScanner;
	delete this->insertedRecordsScanner;
}

void TableTransactionIndexRangeScanner::start(VirtualMachine* vm) {
	CdbTransactionManager* mgr = this->trx->getTrxManager();
	CdbStorageManager* storageManager = mgr->getStorageManager();

	const CdbOid* oid = this->table->getOid();
	this->tableStore = storageManager->getTableStore(oid);

	CdbTableIndex* index = this->indexCandidate->getIndex();
	const CdbOid* indexOid = index->getOid();
	this->indexStore = tableStore->getIndexStore(indexOid);

	// bottom
	const IValueProvider* value = this->indexCandidate->getValue(vm);
	AbstractScanConditionElement* element = dynamic_cast<AbstractScanConditionElement*>(value->clone()); __STP(element);

	AbstractCdbValue* v = element->evaluate(nullptr, nullptr, nullptr); __STP(v);
	CdbRecordKey* beginkey = new CdbRecordKey(); __STP(beginkey);
	beginkey->addKey(v->toKey());
	bool beginEq = this->indexCandidate->isBottomEq(vm);

	// end
	const IValueProvider* topValue = this->indexCandidate->getTopValue(vm);
	AbstractScanConditionElement* topElement = dynamic_cast<AbstractScanConditionElement*>(topValue->clone()); __STP(topElement);

	AbstractCdbValue* tv = topElement->evaluate(nullptr, nullptr, nullptr); __STP(tv);
	CdbRecordKey* endkey = new CdbRecordKey();  __STP(endkey);
	endkey->addKey(tv->toKey());
	bool endEq = this->indexCandidate->isTopEq(vm);

	IndexScanner* s = this->indexStore->getScanner(beginkey, beginEq, endkey, endEq);
	s->start();
	setScanner(s);

	this->insertedRecordsScanner = this->trxCache->getInsertedCacheIndexScanner(this->table, indexOid, beginkey, beginEq, endkey, endEq);
	this->updatedRecordsScanner = this->trxCache->getUpdatedCacheIndexScanner(this->table, indexOid, beginkey, beginEq, endkey, endEq);
}

void TableTransactionIndexRangeScanner::restartScan(VirtualMachine* vm) {
	AbstractLeftSourceTransactionScanner::restartScan(vm);

	// bottom
	const IValueProvider* value = this->indexCandidate->getValue(vm);
	AbstractScanConditionElement* element = dynamic_cast<AbstractScanConditionElement*>(value->clone()); __STP(element);

	AbstractCdbValue* v = element->evaluate(nullptr, nullptr, nullptr); __STP(v);
	CdbRecordKey* beginkey = new CdbRecordKey(); __STP(beginkey);
	beginkey->addKey(v->toKey());
	bool beginEq = this->indexCandidate->isBottomEq(vm);

	// end
	const IValueProvider* topValue = this->indexCandidate->getTopValue(vm);
	AbstractScanConditionElement* topElement = dynamic_cast<AbstractScanConditionElement*>(topValue->clone()); __STP(topElement);

	AbstractCdbValue* tv = topElement->evaluate(nullptr, nullptr, nullptr); __STP(tv);
	CdbRecordKey* endkey = new CdbRecordKey();  __STP(endkey);
	endkey->addKey(tv->toKey());
	bool endEq = this->indexCandidate->isTopEq(vm);

	IndexScanner* s = this->indexStore->getScanner(beginkey, beginEq, endkey, endEq);
	s->start();
	setScanner(s);

	this->scanedScanner = false;
	this->scannedInsertedScanner = false;

	delete this->insertedRecordsScanner, this->insertedRecordsScanner = nullptr;
	delete this->updatedRecordsScanner, this->updatedRecordsScanner = nullptr;

	CdbTableIndex* index = this->indexCandidate->getIndex();
	const CdbOid* indexOid = index->getOid();

	this->insertedRecordsScanner = this->trxCache->getInsertedCacheIndexScanner(this->table, indexOid, beginkey, beginEq, endkey, endEq);
	this->updatedRecordsScanner = this->trxCache->getUpdatedCacheIndexScanner(this->table, indexOid, beginkey, beginEq, endkey, endEq);
}

bool TableTransactionIndexRangeScanner::hasNext(VirtualMachine* vm) {
	if(!this->scanedScanner){
		while(this->scanner->hasNext()){
			delete this->record;
			const CdbOid* recordOid = this->scanner->next();
			this->record = this->tableStore->findRecord(recordOid);

			if(checkUpdated(this->record)){
				continue;
			}
			return true;
		}

		this->scanedScanner = true;
	}

	if(!this->scannedInsertedScanner){
		bool bl = this->insertedRecordsScanner->hasNext();
		if(!bl){
			this->scannedInsertedScanner = true;
		}
		else{
			delete this->record;
			const CdbRecord* r  =this->insertedRecordsScanner->next();
			this->record = dynamic_cast<CdbRecord*>(r->copy());

			return true;
		}
	}

	bool bl = this->updatedRecordsScanner->hasNext();
	if(bl){
		delete this->record;
		const CdbRecord* r  =this->updatedRecordsScanner->next();
		this->record = dynamic_cast<CdbRecord*>(r->copy());
	}

	return bl;
}

const CdbRecord* TableTransactionIndexRangeScanner::next(VirtualMachine* vm) {
	this->record->initScanIdentity();
	return this->record;
}

void TableTransactionIndexRangeScanner::shutdown() {
	setScanner(nullptr);

	this->indexStore = nullptr;

	delete this->record;
	this->record = nullptr;
}

void TableTransactionIndexRangeScanner::setScanner(IndexScanner* scanner) noexcept {
	if(this->scanner != nullptr){
		this->scanner->shutdown();
		delete this->scanner;
	}

	this->scanner = scanner;
}

} /* namespace codablecash */
