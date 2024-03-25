/*
 * InsertRecordRepository.cpp
 *
 *  Created on: 2020/09/28
 *      Author: iizuka
 */

#include "trx/scan/transaction_update_cache/InsertedRecordsRepository.h"

#include "engine/CdbLocalCacheManager.h"

#include "trx/transaction_cache/OidKeyRecordCache.h"

#include "btree/IBtreeScanner.h"

#include "schema_table/record/table_record/CdbRecord.h"

#include "trx/scan/transaction_update_cache_index/TransactionTableUpdateCacheIndexes.h"


namespace codablecash {

InsertedRecordsRepository::InsertedRecordsRepository(CdbStorageManager* storageManager, CdbLocalCacheManager* cacheManager) {
	this->cache = cacheManager->createOidKeyRecordCache();
	this->indexes = new TransactionTableUpdateCacheIndexes(storageManager, this->cache);
	this->cacheManager = cacheManager;
}

InsertedRecordsRepository::~InsertedRecordsRepository() {
	delete this->indexes;
	delete this->cache;
	this->cacheManager = nullptr;
}

void InsertedRecordsRepository::initIndex(const CdbTable* table, CdbTableIndex* index) {
	this->indexes->addIndex(table, index, this->cacheManager);
}

IBtreeScanner* InsertedRecordsRepository::getScanner() {
	return this->cache->getScanner();
}

UpdateCacheIndexScanner* codablecash::InsertedRecordsRepository::getInsertedCacheIndexScanner(
		const CdbOid* indexOid, const CdbRecordKey* begin, bool beginEq,
		const CdbRecordKey* end, bool endEq) {
	return this->indexes->getUpdateCacheIndexScanner(indexOid, begin, beginEq, end, endEq, this->cache);
}


void codablecash::InsertedRecordsRepository::addRecord(const CdbRecord* record) {
	this->cache->insert(record);
	this->indexes->addRecord(record);
}

void InsertedRecordsRepository::removeRecord(const CdbOid* oid) {
	this->indexes->removeRecord(oid);
	this->cache->remove(oid);
}

void InsertedRecordsRepository::updateRecord(const CdbRecord* record) {
	const CdbOid* oid = record->getOid();

	removeRecord(oid);
	addRecord(record);
}


} /* namespace codablecash */
