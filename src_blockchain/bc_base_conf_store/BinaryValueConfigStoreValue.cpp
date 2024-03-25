/*
 * BinaryValue.cpp
 *
 *  Created on: 2023/02/24
 *      Author: iizuka
 */

#include "bc_base_conf_store/BinaryValueConfigStoreValue.h"

#include "base_io/ByteBuffer.h"

#include "osenv/memory.h"

using alinous::Mem;

namespace codablecash {

BinaryValueConfigStoreValue::BinaryValueConfigStoreValue() : AbstractConfigStoreElement(AbstractConfigStoreElement::TYPE_BINARY_VALUE) {
	this->data = nullptr;
	this->length = 0;
}

BinaryValueConfigStoreValue::~BinaryValueConfigStoreValue() {
	delete [] this->data;
}

int BinaryValueConfigStoreValue::binarySize() const noexcept {
	int total = sizeof(uint8_t) + sizeof(int16_t);
	total += this->length;
	return total;
}

void BinaryValueConfigStoreValue::toBinary(ByteBuffer *out) const {
	out->put(this->type);
	out->putShort(this->length);
	out->put((const uint8_t* )this->data, this->length);
}

void BinaryValueConfigStoreValue::init(const uint8_t *data, int length) {
	this->length = length;
	this->data = new char[length];

	Mem::memcpy(this->data, data, length);
}

void BinaryValueConfigStoreValue::fromBinary(ByteBuffer *in) {
	this->length = in->getShort();
	this->data = new char[this->length];

	in->get((uint8_t*)this->data, this->length);

}

} /* namespace codablecash */
