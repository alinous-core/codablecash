/*
 * OkPubsubResponse.cpp
 *
 *  Created on: 2023/01/15
 *      Author: iizuka
 */

#include "pubsub_cmd/OkPubsubResponse.h"

#include "base_io/ByteBuffer.h"

#include "base/UnicodeString.h"

namespace codablecash {

OkPubsubResponse::OkPubsubResponse() : AbstractCommandResponse(AbstractCommandResponse::TYPE_RES_OK) {
}

OkPubsubResponse::~OkPubsubResponse() {

}

void OkPubsubResponse::fromBinary(ByteBuffer *buff) {
}

int OkPubsubResponse::binarySize() const {
	int total = sizeof(this->type);

	return total;
}

void OkPubsubResponse::toBinary(ByteBuffer *buff) const {
	buff->putInt(this->type);
}

alinous::UnicodeString* OkPubsubResponse::toString() const noexcept {
	UnicodeString* str = new UnicodeString(L"OK");

	return str;
}

} /* namespace codablecash */
