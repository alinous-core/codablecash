/*
 * GroupKeyCache.cpp
 *
 *  Created on: 2021/05/20
 *      Author: iizuka
 */

#include "trx/transaction_cache/GroupCache.h"


#include "vm/VirtualMachine.h"



namespace codablecash {

GroupCache::GroupCache(CdbLocalCacheManager* localCacheManager) {
	this->localCacheManager = localCacheManager;
}

GroupCache::~GroupCache() {
	this->localCacheManager = nullptr;
}

void GroupCache::init(VirtualMachine* vm) {
}

void GroupCache::groupRecord(const CdbRecord* record, const ScanResultMetadata* metadata, GroupByPlanner* groupByPlan) {
}


} /* namespace codablecash */
