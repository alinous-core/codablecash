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
	this->nextOid = nullptr;
}

void OidArrayCacheScanner::init(int index) {
	uint64_t fpos = this->cache->getIndexFpos(index);

	// get first
	this->element = this->cache->loadOidArrayElement(fpos);
}

bool OidArrayCacheScanner::hasNext() {
	resetNextObject();

	if(!checkCacheElement()){
		return false;
	}

	this->nextOid = this->element->get(this->cursor);
	this->cursor++;

	return true;
}

bool OidArrayCacheScanner::checkCacheElement() noexcept {
	if(this->element->size() <= this->cursor){
		// new element
		uint64_t nextFpos = this->element->getNextFpos();
		delete this->element;
		this->element = nullptr;

		if(nextFpos == 0){
			return false;
		}

		this->element = this->cache->loadOidArrayElement(nextFpos);
		this->cursor = 0;
	}

	return true;
}

void OidArrayCacheScanner::resetNextObject() noexcept {
	//delete this->nextOid;
	this->nextOid = nullptr;
}

const CdbOid* OidArrayCacheScanner::next() {
	return this->nextOid;
}

} /* namespace codablecash */
