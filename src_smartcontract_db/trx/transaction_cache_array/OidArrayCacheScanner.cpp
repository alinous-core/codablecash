/*
 * OidArrayCacheScanner.cpp
 *
 *  Created on: 2021/06/25
 *      Author: iizuka
 */

#include "trx/transaction_cache_array/OidArrayCacheScanner.h"

namespace codablecash {

OidArrayCacheScanner::OidArrayCacheScanner(OidArrayCache* cache) {
	this->cache = cache;
}

OidArrayCacheScanner::~OidArrayCacheScanner() {
	this->cache = nullptr;
}

} /* namespace codablecash */
