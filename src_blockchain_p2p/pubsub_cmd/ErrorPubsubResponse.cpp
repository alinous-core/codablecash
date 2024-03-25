/*
 * ErrorPubsubResponse.cpp
 *
 *  Created on: 2023/09/13
 *      Author: iizuka
 */

#include "pubsub_cmd/ErrorPubsubResponse.h"

#include "base_io/ByteBuffer.h"

#include "base/UnicodeString.h"

#include "bc_base/BinaryUtils.h"
namespace codablecash {

ErrorPubsubResponse::ErrorPubsubResponse() : AbstractCommandResponse(TYPE_RES_ERROR) {
	this->message = new UnicodeString("Pubsub Handshake Connection Error.");
}

ErrorPubsubResponse::~ErrorPubsubResponse() {
	delete this->message;
}

int ErrorPubsubResponse::binarySize() const {
	BinaryUtils::checkNotNull(this->message);

	int total = sizeof(this->type);
	total += BinaryUtils::stringSize(this->message);

	return total;
}

void ErrorPubsubResponse::toBinary(ByteBuffer *buff) const {
	BinaryUtils::checkNotNull(this->message);

	buff->putInt(this->type);
	BinaryUtils::putString(buff, this->message);
}

void ErrorPubsubResponse::fromBinary(ByteBuffer *buff) {
	delete this->message;
	this->message = nullptr;

	this->message = BinaryUtils::getString(buff);

	BinaryUtils::checkNotNull(this->message);
}

UnicodeString* ErrorPubsubResponse::toString() const noexcept {
	UnicodeString* str = new UnicodeString(L"[Error Respose] ");
	str->append(this->message);

	return str;
}

void ErrorPubsubResponse::setMessage(const UnicodeString *message) noexcept {
	delete this->message;
	this->message = new UnicodeString(message);
}

} /* namespace codablecash */
