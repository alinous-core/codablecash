/*
 * GroupKeyCache.cpp
 *
 *  Created on: 2021/05/20
 *      Author: iizuka
 */

#include "trx/transaction_cache/GroupCache.h"
#include "trx/transaction_cache/GroupRecordCache.h"

#include "trx/transaction_cache_array/OidArrayCache.h"

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
	CdbRecord* newRecord = groupByPlan->groupBy(vm, record, metadata); __STP(newRecord);


}


} /* namespace codablecash */
