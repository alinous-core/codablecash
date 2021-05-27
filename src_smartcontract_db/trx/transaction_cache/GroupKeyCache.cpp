/*
 * GroupKeyCache.cpp
 *
 *  Created on: 2021/05/20
 *      Author: iizuka
 */

#include "trx/transaction_cache/GroupKeyCache.h"

namespace codablecash {

GroupKeyCache::GroupKeyCache(CdbLocalCacheManager* localCacheManager) {
	this->localCacheManager = localCacheManager;
}

GroupKeyCache::~GroupKeyCache() {
	this->localCacheManager = nullptr;
}

} /* namespace codablecash */
