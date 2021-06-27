/*
 * OidArrayCacheElement.cpp
 *
 *  Created on: 2021/06/15
 *      Author: iizuka
 */

#include "trx/transaction_cache_array/OidArrayCacheElement.h"

#include "engine/CdbOid.h"

#include "base_io/ByteBuffer.h"

namespace codablecash {

OidArrayCacheElement::OidArrayCacheElement(int size) {
	this->listSize = size;

	this->fpos = 0;
	this->nextFpos = 0;
}

OidArrayCacheElement::~OidArrayCacheElement() {
	this->list.deleteElements();
}

int OidArrayCacheElement::blockSize() {
	int total = sizeof(uint8_t);

	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		CdbOid* oid = this->list.get(i);
		total += oid->binarySize();
	}

	total += sizeof(this->nextFpos);
	return total;
}

void OidArrayCacheElement::toBinary(ByteBuffer* buff) {
	int maxLoop = this->list.size();
	buff->put(maxLoop);

	for(int i = 0; i != maxLoop; ++i){
		CdbOid* oid = this->list.get(i);

		oid->toBinary(buff);
	}

	buff->putLong(this->nextFpos);
}

OidArrayCacheElement* OidArrayCacheElement::fromBinary(ByteBuffer* buff, int maxsize) {
	OidArrayCacheElement* element = new OidArrayCacheElement(maxsize);

	int maxLoop = (int)buff->get();
	for(int i = 0; i != maxLoop; ++i){
		CdbOid* oid = CdbOid::fromBinary(buff);
		element->list.addElement(oid);
	}

	element->nextFpos = buff->getLong();

	return element;
}

void OidArrayCacheElement::addOid(const CdbOid* oid) {
	this->list.addElement(oid->copy());
}

bool OidArrayCacheElement::isFull() const noexcept {
	return this->list.size() >= this->listSize;
}

int OidArrayCacheElement::size() const noexcept {
	return this->list.size();
}

const CdbOid* OidArrayCacheElement::get(int i) const noexcept {
	return this->list.get(i);
}

} /* namespace codablecash */
