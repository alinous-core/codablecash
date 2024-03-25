/*
 * TransactionTableUpdateCache.cpp
 *
 *  Created on: 2020/06/13
 *      Author: iizuka
 */

#include "trx/scan/transaction_update_cache/TransactionTableUpdateCache.h"
#include "trx/scan/transaction_update_cache/InsertRecordsCacheCursor.h"
#include "trx/scan/transaction_update_cache/InsertedRecordsRepository.h"
#include "trx/scan/transaction_update_cache/UpdatedRecordsRepository.h"
#include "trx/scan/transaction_update_cache/DeletedOidsRepository.h"
#include "trx/scan/transaction_update_cache/UpdatedRecordCursor.h"
#include "trx/scan/transaction_update_cache/DeletedRecordsOidsCursor.h"

#include "schema_table/table/CdbTable.h"

#include "schema_table/record/table_record/CdbRecord.h"

#include "schema_table/record/table_record_key/CdbOidKey.h"
#include "schema_table/record/table_record_key/AbstractCdbKey.h"

#include "base/StackRelease.h"

#include "engine/CdbOid.h"

#include "trx/scan/transaction_update_cache_index/UpdateCacheIndexScanner.h"

namespace codablecash {

TransactionTableUpdateCache::TransactionTableUpdateCache(const CdbTable* table,
				CdbLocalCacheManager* cacheManager, CdbStorageManager* storageManager) {
	this->table = table;
	this->cacheManager = cacheManager;

	this->insertedRecordRepo = new InsertedRecordsRepository(storageManager, cacheManager);
	this->updatedRecordRepo = new UpdatedRecordsRepository(storageManager, cacheManager);
	this->deletedRecordRepo = new DeletedOidsRepository(cacheManager);
}

TransactionTableUpdateCache::~TransactionTableUpdateCache() {
	this->table = nullptr;
	this->cacheManager = nullptr;

	delete this->insertedRecordRepo;
	delete this->updatedRecordRepo;
	delete this->deletedRecordRepo;
}

void TransactionTableUpdateCache::init() {
	const ArrayList<CdbTableIndex, CdbTableIndexCompare>* list = this->table->getIndexes();

	int maxLoop = list->size();
	for(int i = 0; i != maxLoop; ++i){
		CdbTableIndex* index = list->get(i);

		this->updatedRecordRepo->initIndex(this->table, index);
		this->insertedRecordRepo->initIndex(this->table, index);
	}
}


void TransactionTableUpdateCache::addInsertedRecord(const CdbRecord* newRecord) noexcept {
	this->insertedRecordRepo->addRecord(newRecord);
}

void TransactionTableUpdateCache::removeInsertedRecord(const CdbOid* oid) noexcept {
	this->insertedRecordRepo->removeRecord(oid);
}

void TransactionTableUpdateCache::updateInsertedRecord(const CdbRecord* updatedRecord) noexcept {
	this->insertedRecordRepo->updateRecord(updatedRecord);
}

InsertRecordsCacheCursor* TransactionTableUpdateCache::newInsertedRecordCursor() const noexcept {
	return new InsertRecordsCacheCursor(this->insertedRecordRepo);
}

void TransactionTableUpdateCache::addDeletedRecord(const CdbOid* recordOid) {
	this->deletedRecordRepo->addDeletedRecord(recordOid);
}

bool TransactionTableUpdateCache::isDeleted(const CdbOid* recordOid) {
	AbstractCdbKey* abkey = recordOid->toKey(); __STP(abkey);
	CdbOidKey* key = dynamic_cast<CdbOidKey*>(abkey);

	return isDeleted(key);
}

bool TransactionTableUpdateCache::isDeleted(const CdbOidKey* recordOidKey) {
	return this->deletedRecordRepo->isDeleted(recordOidKey);
}

DeletedRecordsOidsCursor* TransactionTableUpdateCache::getDeletedRecordsOidsCursor() {
	return new DeletedRecordsOidsCursor(this->deletedRecordRepo);
}

void TransactionTableUpdateCache::addUpdatedRecord(const CdbRecord* updatedRecord) {
	this->updatedRecordRepo->addUpdatedRecord(updatedRecord);
}

bool TransactionTableUpdateCache::isUpdated(const CdbOid* recordOid) {
	AbstractCdbKey* abkey = recordOid->toKey(); __STP(abkey);
	CdbOidKey* key = dynamic_cast<CdbOidKey*>(abkey);

	return isUpdated(key);
}

bool TransactionTableUpdateCache::isUpdated(const CdbOidKey* recordOidKey) {
	return this->updatedRecordRepo->isUpdated(recordOidKey);
}

const CdbRecord* TransactionTableUpdateCache::getUpdatedRecord(const CdbOid* recordOid) {
	AbstractCdbKey* abkey = recordOid->toKey(); __STP(abkey);
	CdbOidKey* key = dynamic_cast<CdbOidKey*>(abkey);

	return getUpdatedRecord(key);
}

const CdbRecord* TransactionTableUpdateCache::getUpdatedRecord(const CdbOidKey* recordOidKey) {
	return this->updatedRecordRepo->getUpdatedRecord(recordOidKey);
}

UpdatedRecordCursor* TransactionTableUpdateCache::getUpdatedRecordCursor() {
	return new UpdatedRecordCursor(this->updatedRecordRepo);
}

UpdateCacheIndexScanner* TransactionTableUpdateCache::getInsertedCacheIndexScanner(
		const CdbOid* indexOid, const CdbRecordKey* begin, bool beginEq,
		const CdbRecordKey* end, bool endEq) {
	return this->insertedRecordRepo->getInsertedCacheIndexScanner(indexOid, begin, beginEq, end, endEq);
}

UpdateCacheIndexScanner* TransactionTableUpdateCache::getUpdatedCacheIndexScanner(
		const CdbOid* indexOid, const CdbRecordKey* begin, bool beginEq,
		const CdbRecordKey* end, bool endEq) {
	return this->updatedRecordRepo->getUpdatedCacheIndexScanner(indexOid, begin, beginEq, end, endEq);
}

} /* namespace codablecash */
