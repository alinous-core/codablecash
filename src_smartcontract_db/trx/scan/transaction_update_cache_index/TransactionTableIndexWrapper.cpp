/*
 * TransactionTableIndexWrapper.cpp
 *
 *  Created on: 2022/01/12
 *      Author: iizuka
 */

#include "trx/scan/transaction_update_cache_index/TransactionTableIndexWrapper.h"
#include "trx/scan/transaction_update_cache_index/UpdateCacheIndexScanner.h"

#include "schema_table/table/CdbTable.h"
#include "schema_table/table/CdbTableIndex.h"

#include "schema_table/table_store/CdbStorageManager.h"
#include "schema_table/table_store/TableStore.h"
#include "schema_table/table_store/IndexStore.h"

#include "schema_table/record/table_record/CdbRecord.h"

#include "schema_table/record/table_record_key/CdbRecordKey.h"

#include "base/StackRelease.h"

#include "trx/transaction_cache/SingleKeyOidCache.h"

#include "engine/CdbLocalCacheManager.h"


namespace codablecash {

TransactionTableIndexWrapper::TransactionTableIndexWrapper(const CdbTable* table, CdbTableIndex* index, CdbStorageManager* storageManager) {
	this->index = index;

	const CdbOid* tableOid = table->getOid();
	TableStore* tableStore = storageManager->getTableStore(tableOid);

	const CdbOid* indexOid = this->index->getOid();
	this->indexStore = tableStore->getIndexStore(indexOid);

	this->keyOidCache = nullptr;
}

TransactionTableIndexWrapper::~TransactionTableIndexWrapper() {
	this->index = nullptr;
	this->indexStore = nullptr;

	delete this->keyOidCache;
}

void TransactionTableIndexWrapper::init(CdbLocalCacheManager* localCacheManager) {
	this->keyOidCache = localCacheManager->createSingleKeyOidCache();
}

void TransactionTableIndexWrapper::addRecord(const CdbRecord* record) {
	CdbRecordKey* key = this->indexStore->makeKey(record); __STP(key);

	this->keyOidCache->insert(key, record->getOid());
}

void TransactionTableIndexWrapper::removeRecord(const CdbOid* oid, const CdbRecord* record) {
	CdbRecordKey* key = this->indexStore->makeKey(record); __STP(key);

	this->keyOidCache->remove(key, record->getOid());
}

UpdateCacheIndexScanner* TransactionTableIndexWrapper::getUpdateCacheIndexScanner(
		const CdbRecordKey* begin, bool beginEq, const CdbRecordKey* end, bool endEq, OidKeyRecordCache* recordCache) {
	UpdateCacheIndexScanner* scanner = new UpdateCacheIndexScanner(this->keyOidCache, begin, beginEq, end, endEq, recordCache);
	scanner->init();

	return scanner;
}


} /* namespace codablecash */
