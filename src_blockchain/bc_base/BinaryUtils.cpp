/*
 * BinaryUtils.cpp
 *
 *  Created on: 2023/02/20
 *      Author: iizuka
 */

#include "bc_base/BinaryUtils.h"

#include "filestore_block/exceptions.h"

#include "base_io/ByteBuffer.h"

#include "base/UnicodeString.h"

namespace codablecash {

void BinaryUtils::checkNotNull(void *obj) {
	if(obj == nullptr){
		throw new BinaryFormatException(__FILE__, __LINE__);
	}
}

void BinaryUtils::checkUShortRange(uint16_t value, uint16_t min, uint16_t max) {
	if(min > value || value > max){
		throw new BinaryFormatException(__FILE__, __LINE__);
	}
}

void BinaryUtils::checkUint8Value(uint8_t value, uint8_t correctValue) {
	if(value != correctValue){
		throw new BinaryFormatException(__FILE__, __LINE__);
	}
}

int BinaryUtils::stringSize(const UnicodeString *str) noexcept {
	return sizeof(uint32_t) + str->length() * sizeof(uint16_t);
}

void BinaryUtils::putString(ByteBuffer *out, const UnicodeString *str) noexcept {
	uint32_t maxLoop = str->length();
	out->putInt(maxLoop);

	for(int i = 0; i != maxLoop; ++i){
		wchar_t ch = str->get(i);
		out->putChar(ch);
	}
}

UnicodeString* BinaryUtils::getString(ByteBuffer *in) noexcept {
	UnicodeString* ret = new UnicodeString(L"");
	uint32_t maxLoop = in->getInt();
	for(int i = 0; i != maxLoop; ++i){
		wchar_t ch = in->getChar();
		ret->append(ch);
	}

	return ret;
}

} /* namespace codablecash */
