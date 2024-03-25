/*
 * PubsubUtils.cpp
 *
 *  Created on: 2023/01/13
 *      Author: iizuka
 */

#include "pubsub/PubsubUtils.h"

#include <cstdint>

#include "base_io/ByteBuffer.h"

#include "base/UnicodeString.h"

#include "ipconnect/IClientSocket.h"

#include "base/StackRelease.h"

#include "pubsub/PubsubCommandException.h"
namespace codablecash {

void PubsubUtils::putString(ByteBuffer* out, const UnicodeString* str) noexcept {
	uint32_t maxLoop = str->length();
	out->putInt(maxLoop);

	for(int i = 0; i != maxLoop; ++i){
		wchar_t ch = str->get(i);
		out->putChar(ch);
	}
}

UnicodeString* PubsubUtils::getString(ByteBuffer* in) noexcept {
	UnicodeString* ret = new UnicodeString(L"");
	uint32_t maxLoop = in->getInt();
	for(int i = 0; i != maxLoop; ++i){
		wchar_t ch = in->getChar();
		ret->append(ch);
	}

	return ret;
}

int PubsubUtils::stringSize(const UnicodeString* str) noexcept {
	return sizeof(uint32_t) + str->length() * sizeof(uint16_t);
}

ByteBuffer* PubsubUtils::readAll(IClientSocket *client, int size) {
	char* b = new char[size]{};
	StackArrayRelease<char> st_b(b);

	int length = client->read(b, size);
	if(length != size){
		throw new PubsubCommandException(L"Failed in reading command.", __FILE__, __LINE__);
	}

	ByteBuffer* buff = ByteBuffer::wrapWithEndian((const uint8_t*)b, length, true);

	return buff;
}

} /* namespace codablecash */
