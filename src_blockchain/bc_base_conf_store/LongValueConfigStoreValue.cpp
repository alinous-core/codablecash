/*
 * LongValueConfigStoreValue.cpp
 *
 *  Created on: 2022/05/14
 *      Author: iizuka
 */

#include "bc_base_conf_store/LongValueConfigStoreValue.h"

#include "base_io/ByteBuffer.h"

namespace codablecash {

LongValueConfigStoreValue::LongValueConfigStoreValue() : AbstractConfigStoreElement(AbstractConfigStoreElement::TYPE_LONG_VALUE) {
	this->value = 0;
}

LongValueConfigStoreValue::LongValueConfigStoreValue(uint64_t value) : AbstractConfigStoreElement(AbstractConfigStoreElement::TYPE_LONG_VALUE) {
	this->value = value;
}

LongValueConfigStoreValue::~LongValueConfigStoreValue() {

}

int LongValueConfigStoreValue::binarySize() const noexcept {
	return sizeof(uint8_t) + sizeof(uint64_t);
}

void LongValueConfigStoreValue::toBinary(ByteBuffer* out) const {
	out->put(this->type);
	out->putLong(this->value);
}

void LongValueConfigStoreValue::fromBinary(ByteBuffer* in) {
	this->value = in->getLong();
}

} /* namespace codablecash */
