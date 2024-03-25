/*
 * ShortValueConfigStoreValue.cpp
 *
 *  Created on: 2023/01/25
 *      Author: iizuka
 */

#include "bc_base_conf_store/ShortValueConfigStoreValue.h"

#include "base_io/ByteBuffer.h"

namespace codablecash {

ShortValueConfigStoreValue::ShortValueConfigStoreValue() : AbstractConfigStoreElement(AbstractConfigStoreElement::TYPE_SHORT_VALUE) {
	this->value = 0;
}

ShortValueConfigStoreValue::ShortValueConfigStoreValue(int16_t value) : AbstractConfigStoreElement(AbstractConfigStoreElement::TYPE_SHORT_VALUE) {
	this->value = value;
}

ShortValueConfigStoreValue::~ShortValueConfigStoreValue() {

}

int ShortValueConfigStoreValue::binarySize() const noexcept {
	return sizeof(uint8_t) + sizeof(int16_t);
}

void ShortValueConfigStoreValue::toBinary(ByteBuffer *out) const {
	out->put(this->type);
	out->putShort(this->value);
}

void ShortValueConfigStoreValue::fromBinary(ByteBuffer *in) {
	this->value = in->getShort();
}

} /* namespace codablecash */
