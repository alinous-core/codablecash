/*
 * OidArrayCacheScanner.cpp
 *
 *  Created on: 2021/06/25
 *      Author: iizuka
 */

#include "trx/transaction_cache_array/OidArrayCacheScanner.h"
#include "trx/transaction_cache_array/OidArrayCache.h"
#include "trx/transaction_cache_array/OidArrayCacheElement.h"

#include "engine/CdbOid.h"

namespace codablecash {

OidArrayCacheScanner::OidArrayCacheScanner(OidArrayCache* cache) {
	this->cache = cache;
	this->element = nullptr;
	this->cursor = 0;
	this->nextOid = nullptr;
}

OidArrayCacheScanner::~OidArrayCacheScanner() {
	this->cache = nullptr;
	delete this->element;
	delete this->nextOid;
}

void OidArrayCacheScanner::init(int index) {
	uint64_t fpos = this->cache->getIndexFpos(index);

	// get first
	this->element = this->cache->loadOidArrayElement(fpos);
}

bool OidArrayCacheScanner::hasNext() {
	resetNextObject();

	if(this->element->size() <= this->cursor){

	}


	return false;
}

void OidArrayCacheScanner::resetNextObject() noexcept {
	delete this->nextOid;
	this->nextOid = nullptr;
}

CdbOid* OidArrayCacheScanner::next() {
	return nullptr;
}

} /* namespace codablecash */
