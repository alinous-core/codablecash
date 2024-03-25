/*
 * TableTransactionIndexScanner.cpp
 *
 *  Created on: 2020/12/10
 *      Author: iizuka
 */

#include "trx/scan/transaction_scanner/TableTransactionIndexScanner.h"

#include "scan_select/scan_condition/base/IValueProvider.h"

#include "schema_table/table_store/TableStore.h"
#include "schema_table/table_store/CdbStorageManager.h"
#include "schema_table/table_store/IndexStore.h"

#include "schema_table/table/CdbTable.h"
#include "schema_table/table/CdbTableIndex.h"

#include "trx/transaction/CdbTransactionManager.h"
#include "trx/transaction/CdbTransaction.h"

#include "scan_select/scan/IndexScanner.h"

#include "scan_select/scan_condition/base/AbstractScanConditionElement.h"

#include "base/StackRelease.h"
#include "scan_select/scan_planner/scanner/index_resolv/AbstractColumnsIndexAccess.h"

#include "schema_table/record/table_record_value/AbstractCdbValue.h"
#include "schema_table/record/table_record_key/CdbRecordKey.h"

#include "schema_table/record/table_record/CdbRecord.h"

#include "trx/scan/transaction_update_cache/TransactionUpdateCache.h"

#include "trx/scan/transaction_update_cache_index/UpdateCacheIndexScanner.h"

namespace codablecash {

TableTransactionIndexScanner::TableTransactionIndexScanner(ScanResultMetadata* metadata,
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

TableTransactionIndexScanner::~TableTransactionIndexScanner() {
	shutdown();

	delete this->indexCandidate;

	delete this->updatedRecordsScanner;
	delete this->insertedRecordsScanner;
}

void TableTransactionIndexScanner::start(VirtualMachine* vm) {
	CdbTransactionManager* mgr = this->trx->getTrxManager();
	CdbStorageManager* storageManager = mgr->getStorageManager();

	const CdbOid* oid = this->table->getOid();
	this->tableStore = storageManager->getTableStore(oid);

	CdbTableIndex* index = this->indexCandidate->getIndex();
	const CdbOid* indexOid = index->getOid();
	this->indexStore = tableStore->getIndexStore(indexOid);

	const IValueProvider* value = this->indexCandidate->getValue(vm);
	AbstractScanConditionElement* element = dynamic_cast<AbstractScanConditionElement*>(value->clone()); __STP(element);

	const ScanResultMetadata* meta = getMetadata();

	AbstractCdbValue* v = element->evaluate(vm, nullptr, meta); __STP(v);
	CdbRecordKey* rkey = new CdbRecordKey(); __STP(rkey);
	rkey->addKey(v->toKey());

	IndexScanner* s = this->indexStore->getScanner(rkey, true, rkey, true);
	s->start();
	setScanner(s);

	this->insertedRecordsScanner = this->trxCache->getInsertedCacheIndexScanner(this->table, indexOid, rkey, true, rkey, true);
	this->updatedRecordsScanner = this->trxCache->getUpdatedCacheIndexScanner(this->table, indexOid, rkey, true, rkey, true);

}

void TableTransactionIndexScanner::restartScan(VirtualMachine* vm) {
	AbstractLeftSourceTransactionScanner::restartScan(vm);

	const IValueProvider* value = this->indexCandidate->getValue(vm);
	AbstractScanConditionElement* element = dynamic_cast<AbstractScanConditionElement*>(value->clone()); __STP(element);

	AbstractCdbValue* v = element->evaluate(nullptr, nullptr, nullptr); __STP(v);
	CdbRecordKey* rkey = new CdbRecordKey(); __STP(rkey);
	rkey->addKey(v->toKey());

	IndexScanner* s = this->indexStore->getScanner(rkey, true, rkey, true);
	s->start();
	setScanner(s);

	delete this->insertedRecordsScanner, this->insertedRecordsScanner = nullptr;
	delete this->updatedRecordsScanner, this->updatedRecordsScanner = nullptr;

	CdbTableIndex* index = this->indexCandidate->getIndex();
	const CdbOid* indexOid = index->getOid();

	this->insertedRecordsScanner = this->trxCache->getInsertedCacheIndexScanner(this->table, indexOid, rkey, true, rkey, true);
	this->updatedRecordsScanner = this->trxCache->getUpdatedCacheIndexScanner(this->table, indexOid, rkey, true, rkey, true);
}

bool TableTransactionIndexScanner::hasNext(VirtualMachine* vm) {
	if(!this->scanedScanner){
		while(this->scanner->hasNext()){
			delete this->record;
			const CdbOid* recordOid = this->scanner->next();
			this->record = this->tableStore->findRecord(recordOid);
			this->record->initScanIdentity();

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
			const CdbRecord* r = this->insertedRecordsScanner->next();
			this->record = dynamic_cast<CdbRecord*>(r->copy());
			this->record->initScanIdentity();

			return true;
		}
	}

	bool bl = this->updatedRecordsScanner->hasNext();
	if(bl){
		delete this->record;
		const CdbRecord* r  =this->updatedRecordsScanner->next();
		this->record = dynamic_cast<CdbRecord*>(r->copy());
		this->record->initScanIdentity();
	}

	return bl;
}

const CdbRecord* TableTransactionIndexScanner::next(VirtualMachine* vm) {
	return this->record;
}

void TableTransactionIndexScanner::shutdown() {
	setScanner(nullptr);

	this->indexStore = nullptr;

	delete this->record;
	this->record = nullptr;
}

void TableTransactionIndexScanner::setScanner(IndexScanner* scanner) noexcept {
	if(this->scanner != nullptr){
		this->scanner->shutdown();
		delete this->scanner;
	}

	this->scanner = scanner;
}

} /* namespace codablecash */
