/*
 * RightTableIndexTransactionScanner.cpp
 *
 *  Created on: 2020/12/20
 *      Author: iizuka
 */

#include "trx/scan/transaction_scanner_join_right/RightTableIndexTransactionScanner.h"

#include "trx/scan/transaction_update_cache_index/UpdateCacheIndexScanner.h"
#include "trx/scan/transaction_update_cache/TransactionUpdateCache.h"

#include "schema_table/table/CdbTableIndex.h"
#include "schema_table/table/CdbTable.h"

#include "schema_table/table_store/TableStore.h"
#include "schema_table/table_store/IndexStore.h"
#include "schema_table/table_store/CdbStorageManager.h"

#include "schema_table/record/table_record_key/CdbRecordKey.h"

#include "trx/transaction/CdbTransaction.h"
#include "trx/transaction/CdbTransactionManager.h"

#include "scan_select/scan/IndexScanner.h"

#include "schema_table/record/table_record/CdbRecord.h"


namespace codablecash {

RightTableIndexTransactionScanner::RightTableIndexTransactionScanner(ScanResultMetadata* metadata, CdbTransaction* trx,
		const CdbTable* table, const AbstractScanCondition* filterCondition, CdbTableIndex* index)
			: AbstractRightTransactionScanner(metadata, trx, table, filterCondition){
	this->index = index;
	this->tableStore = nullptr;
	this->indexStore = nullptr;
	this->scanner = nullptr;
	this->record = nullptr;

	this->scanedScanner = false;
	this->insertedRecordsScanner = nullptr;
	this->scannedInsertedScanner = false;
	this->updatedRecordsScanner = nullptr;
}

RightTableIndexTransactionScanner::~RightTableIndexTransactionScanner() {
	shutdown();

	this->index = nullptr;

	delete this->updatedRecordsScanner;
	delete this->insertedRecordsScanner;
}

void RightTableIndexTransactionScanner::start(VirtualMachine* vm) {
	CdbTransactionManager* mgr = this->trx->getTrxManager();
	CdbStorageManager* storageManager = mgr->getStorageManager();

	const CdbOid* oid = this->table->getOid();
	this->tableStore = storageManager->getTableStore(oid);

	const CdbOid* indexOid = this->index->getOid();
	this->indexStore = this->tableStore->getIndexStore(indexOid);
}

void RightTableIndexTransactionScanner::reset(const AbstractCdbKey* key) {
	const CdbRecordKey* rkey = dynamic_cast<const CdbRecordKey*>(key);

	IndexScanner* s = this->indexStore->getScanner(rkey, true, rkey, true);
	s->start();
	setScanner(s);

	// use transaction
	const CdbOid* indexOid = this->index->getOid();

	delete this->insertedRecordsScanner, this->insertedRecordsScanner = nullptr;
	delete this->updatedRecordsScanner, this->updatedRecordsScanner = nullptr;

	this->insertedRecordsScanner = this->trxCache->getInsertedCacheIndexScanner(this->table, indexOid, rkey, true, rkey, true);
	this->updatedRecordsScanner = this->trxCache->getUpdatedCacheIndexScanner(this->table, indexOid, rkey, true, rkey, true);

	this->scanedScanner = false;
	this->scannedInsertedScanner = false;
}

bool RightTableIndexTransactionScanner::hasNext(VirtualMachine* vm) {
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

const CdbRecord* RightTableIndexTransactionScanner::next(VirtualMachine* vm) {
	delete this->record;

	const CdbOid* recordOid = this->scanner->next();

	this->record = this->tableStore->findRecord(recordOid);
	this->record->initScanIdentity();

	return this->record;
}

void RightTableIndexTransactionScanner::shutdown() {
	setScanner(nullptr);
	delete this->record;
	this->record = nullptr;
}

void RightTableIndexTransactionScanner::setScanner(IndexScanner* scanner) noexcept {
	if(this->scanner != nullptr){
		this->scanner->shutdown();
		delete this->scanner;
	}

	this->scanner = scanner;
}

} /* namespace codablecash */
