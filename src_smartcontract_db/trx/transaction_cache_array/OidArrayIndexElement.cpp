/*
 * OidArrayIndexElement.cpp
 *
 *  Created on: 2021/06/17
 *      Author: iizuka
 */

#include "trx/transaction_cache_array/OidArrayIndexElement.h"

namespace codablecash {

OidArrayIndexElement::OidArrayIndexElement(int numElements) {
	this->numElements = numElements;

	this->fpos = 0;
	this->elementsPos = new uint64_t[this->numElements];

	this->nextFpos = 0;
}

OidArrayIndexElement::~OidArrayIndexElement() {
	delete [] this->elementsPos;
}

} /* namespace codablecash */
