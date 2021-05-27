/*
 * GroupKeyCache.cpp
 *
 *  Created on: 2021/05/20
 *      Author: iizuka
 */

#include "trx/transaction_cache/GroupCache.h"

namespace codablecash {

GroupCache::GroupCache(CdbLocalCacheManager* localCacheManager) {
	this->localCacheManager = localCacheManager;
}

GroupCache::~GroupCache() {
	this->localCacheManager = nullptr;
}

} /* namespace codablecash */
