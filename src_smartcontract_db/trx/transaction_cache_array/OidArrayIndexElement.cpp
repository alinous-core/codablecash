/*
 * OidArrayIndexElement.cpp
 *
 *  Created on: 2021/06/17
 *      Author: iizuka
 */

#include "trx/transaction_cache_array/OidArrayIndexElement.h"

#include "base_io/ByteBuffer.h"

namespace codablecash {

OidArrayIndexElement::OidArrayIndexElement(int numElements) {
	this->numElements = numElements;

	this->fpos = 0;
	this->elementsPos = new uint64_t[this->numElements]{};

	this->nextFpos = 0;
}

OidArrayIndexElement::~OidArrayIndexElement() {
	delete [] this->elementsPos;
}

int OidArrayIndexElement::blockSize() {
	int total = sizeof(this->fpos);

	total += sizeof(uint8_t);
	total += sizeof(uint64_t) * this->numElements;

	total += sizeof(this->nextFpos);

	return total;
}

void OidArrayIndexElement::toBinary(ByteBuffer* buff) {
	buff->putLong(this->fpos);

	buff->put(this->numElements);

	for(int i = 0; i != this->numElements; ++i){
		buff->putLong(this->elementsPos[i]);
	}

	buff->putLong(this->nextFpos);
}

} /* namespace codablecash */
