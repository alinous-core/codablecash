/*
 * GroupKeyCache.cpp
 *
 *  Created on: 2021/05/20
 *      Author: iizuka
 */

#include "trx/transaction_cache/GroupCache.h"
#include "trx/transaction_cache/GroupRecordCache.h"
#include "trx/transaction_cache/GroupCacheScanner.h"

#include "trx/transaction_cache_array/OidArrayCache.h"
#include "trx/transaction_cache_array/OidArrayCacheScanner.h"

#include "trx/scan/transaction_scan_result/ScanResultMetadata.h"

#include "scan_select/scan_planner/base/GroupByPlanner.h"

#include "schema_table/record/table_record/CdbRecord.h"
#include "schema_table/record/table_record/CdbGroupedRecord.h"

#include "vm/VirtualMachine.h"

#include "base/StackRelease.h"

#include "engine/CdbLocalCacheManager.h"

namespace codablecash {

GroupCache::GroupCache(CdbLocalCacheManager* localCacheManager) {
	this->localCacheManager = localCacheManager;
	this->arrayCache = nullptr;
	this->cache = nullptr;
}

GroupCache::~GroupCache() {
	this->localCacheManager = nullptr;

	delete this->cache;
	delete this->arrayCache;
}

void GroupCache::init(VirtualMachine* vm) {
	this->cache = this->localCacheManager->createGroupRecordCache();
	this->arrayCache = this->localCacheManager->createOidArrayCache();
}

void GroupCache::groupRecord(VirtualMachine* vm, const CdbRecord* record, const ScanResultMetadata* metadata, GroupByPlanner* groupByPlan) {
	CdbRecord* groupedRecord = groupByPlan->groupBy(vm, record, metadata); __STP(groupedRecord);

	uint64_t index = this->cache->getRecordIndex(groupedRecord);

	const CdbOid* originalOid = record->getOid();
	this->arrayCache->add(index, originalOid);
}

GroupCacheScanner* GroupCache::getScanner() {
	GroupCacheScanner* scanner = new GroupCacheScanner(this);
	scanner->init();

	return scanner;
}

void GroupCache::addGroupedRecord(const CdbGroupedRecord* grecord, GroupRecordCache* lastCache) {
	uint64_t newindex = this->cache->getRecordIndex(grecord);

	OidArrayCache* oidCache = grecord->getOidCache();

	uint64_t lastIndex = lastCache->getRecordIndex(grecord);

	OidArrayCacheScanner* scanner = oidCache->getScanner(lastIndex); __STP(scanner);
	while(scanner->hasNext()){
		const CdbOid* oid = scanner->next();

		this->arrayCache->add(newindex, oid);
	}
}

uint64_t GroupCache::getRecordIndex(const CdbRecord* groupedRecord) {
	uint64_t index = this->cache->getRecordIndex(groupedRecord);

	return index;
}


} /* namespace codablecash */
