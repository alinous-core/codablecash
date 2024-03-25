/*
 * UpdateRecordRepository.cpp
 *
 *  Created on: 2020/09/28
 *      Author: iizuka
 */

#include "trx/scan/transaction_update_cache/UpdatedRecordsRepository.h"

#include "trx/scan/transaction_update_cache_index/TransactionTableUpdateCacheIndexes.h"

#include "trx/transaction_cache/OidKeyRecordCache.h"

#include "engine/CdbLocalCacheManager.h"

#include "schema_table/record/table_record_key/CdbOidKey.h"

#include "schema_table/record/table_record/CdbRecord.h"


namespace codablecash {

UpdatedRecordsRepository::UpdatedRecordsRepository(CdbStorageManager* storageManager, CdbLocalCacheManager* cacheManager) {
	this->cache = cacheManager->createOidKeyRecordCache();
	this->indexes = new TransactionTableUpdateCacheIndexes(storageManager, this->cache);
	this->cacheManager = cacheManager;
}

UpdatedRecordsRepository::~UpdatedRecordsRepository() {
	delete this->indexes;
	delete this->cache;
	this->cacheManager = nullptr;
}

void UpdatedRecordsRepository::initIndex(const CdbTable* table, CdbTableIndex* index) {
	this->indexes->addIndex(table, index, this->cacheManager);
}

IBtreeScanner* UpdatedRecordsRepository::getScanner() {
	return this->cache->getScanner();
}

UpdateCacheIndexScanner* codablecash::UpdatedRecordsRepository::getUpdatedCacheIndexScanner(
		const CdbOid* indexOid, const CdbRecordKey* begin, bool beginEq,
		const CdbRecordKey* end, bool endEq) {
	return this->indexes->getUpdateCacheIndexScanner(indexOid, begin, beginEq, end, endEq, this->cache);
}

void UpdatedRecordsRepository::addUpdatedRecord(const CdbRecord* updatedRecord) {
	this->cache->insert(updatedRecord);
	this->indexes->addRecord(updatedRecord);
}

bool UpdatedRecordsRepository::isUpdated(const CdbOidKey* recordOidKey) {
	return this->cache->hasKey(recordOidKey);
}

const CdbRecord* UpdatedRecordsRepository::getUpdatedRecord(const CdbOidKey* recordOidKey) {
	return this->cache->findRecord(recordOidKey);
}


} /* namespace codablecash */
