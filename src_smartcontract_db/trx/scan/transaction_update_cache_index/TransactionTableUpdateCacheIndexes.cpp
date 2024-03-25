/*
 * TransactionTableUpdateCacheIndexes.cpp
 *
 *  Created on: 2022/01/12
 *      Author: iizuka
 */

#include "trx/scan/transaction_update_cache_index/TransactionTableUpdateCacheIndexes.h"

#include "trx/scan/transaction_update_cache_index/TransactionTableIndexWrapper.h"

#include "trx/transaction_cache/OidKeyRecordCache.h"

#include "engine/CdbOid.h"

#include "schema_table/record/table_record_key/CdbOidKey.h"
#include "schema_table/record/table_record_key/AbstractCdbKey.h"

#include "schema_table/table/CdbTableIndex.h"

#include "base/StackRelease.h"
namespace codablecash {

TransactionTableUpdateCacheIndexes::TransactionTableUpdateCacheIndexes(CdbStorageManager* storageManager, OidKeyRecordCache* cache) {
	this->storageManager = storageManager;
	this->cache = cache;
}

TransactionTableUpdateCacheIndexes::~TransactionTableUpdateCacheIndexes() {
	this->list.deleteElements();

	this->storageManager = nullptr;
	this->cache = nullptr;
}

void TransactionTableUpdateCacheIndexes::addIndex(const CdbTable* table, CdbTableIndex* index, CdbLocalCacheManager* localCacheManager) {
	TransactionTableIndexWrapper* wrapper = new TransactionTableIndexWrapper(table, index, this->storageManager);
	wrapper->init(localCacheManager);

	this->list.addElement(wrapper);
}

void TransactionTableUpdateCacheIndexes::addRecord(const CdbRecord* record) {
	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		TransactionTableIndexWrapper* indexWrapper = this->list.get(i);

		indexWrapper->addRecord(record);
	}
}

void TransactionTableUpdateCacheIndexes::removeRecord(const CdbOid* oid) {
	AbstractCdbKey* key = oid->toKey(); __STP(key);
	const CdbRecord* record = this->cache->findRecord(dynamic_cast<CdbOidKey*>(key));

	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		TransactionTableIndexWrapper* indexWrapper = this->list.get(i);

		indexWrapper->removeRecord(oid, record);
	}
}

TransactionTableIndexWrapper* TransactionTableUpdateCacheIndexes::getIndexWrapper(const CdbOid* indexOid) const noexcept {
	TransactionTableIndexWrapper* indexWrapper = nullptr;

	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		TransactionTableIndexWrapper* wr = this->list.get(i);
		CdbTableIndex* index = wr->getIndex();

		const CdbOid* oid = index->getOid();
		if(oid->equals(indexOid)){
			indexWrapper = wr;
			break;
		}
	}

	return indexWrapper;
}

UpdateCacheIndexScanner* TransactionTableUpdateCacheIndexes::getUpdateCacheIndexScanner(
		const CdbOid* indexOid, const CdbRecordKey* begin, bool beginEq,
		const CdbRecordKey* end, bool endEq, OidKeyRecordCache* recordCache) {
	TransactionTableIndexWrapper* indexWrapper = getIndexWrapper(indexOid);

	return indexWrapper->getUpdateCacheIndexScanner(begin, beginEq, end, endEq, recordCache);
}

} /* namespace codablecash */
