/*
 * OidArrayCacheElement.cpp
 *
 *  Created on: 2021/06/15
 *      Author: iizuka
 */

#include "trx/transaction_cache_array/OidArrayCacheElement.h"

#include "engine/CdbOid.h"

namespace codablecash {

OidArrayCacheElement::OidArrayCacheElement(int size) {
	this->size = size;

	this->fpos = 0;
	this->nextFpos = 0;
}

OidArrayCacheElement::~OidArrayCacheElement() {
	this->list.deleteElements();
}

} /* namespace codablecash */
