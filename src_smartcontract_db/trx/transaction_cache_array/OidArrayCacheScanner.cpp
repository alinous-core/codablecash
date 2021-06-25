/*
 * OidArrayCacheScanner.cpp
 *
 *  Created on: 2021/06/25
 *      Author: iizuka
 */

#include "trx/transaction_cache_array/OidArrayCacheScanner.h"
#include "trx/transaction_cache_array/OidArrayCache.h"
#include "trx/transaction_cache_array/OidArrayCacheElement.h"

namespace codablecash {

OidArrayCacheScanner::OidArrayCacheScanner(OidArrayCache* cache) {
	this->cache = cache;
}

OidArrayCacheScanner::~OidArrayCacheScanner() {
	this->cache = nullptr;
}

void OidArrayCacheScanner::init(int index) {
	uint64_t fpos = this->cache->getIndexFpos(index);

	// get first
	OidArrayCacheElement* element = this->cache->loadOidArrayElement(fpos);





}

bool OidArrayCacheScanner::hasNext() {
	return false;
}

CdbOid* OidArrayCacheScanner::next() {
	return nullptr;
}

} /* namespace codablecash */
