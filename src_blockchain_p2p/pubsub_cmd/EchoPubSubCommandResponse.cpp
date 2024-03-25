/*
 * EchoPubSubCommandResponse.cpp
 *
 *  Created on: 2023/01/13
 *      Author: iizuka
 */

#include "pubsub_cmd/EchoPubSubCommandResponse.h"

#include "pubsub/PubsubUtils.h"

#include "base/UnicodeString.h"

#include "base_io/ByteBuffer.h"

namespace codablecash {

EchoPubSubCommandResponse::EchoPubSubCommandResponse() : AbstractCommandResponse(AbstractCommandResponse::TYPE_RES_ECHO) {
	this->message = new UnicodeString(L"");
}

EchoPubSubCommandResponse::~EchoPubSubCommandResponse() {
	delete message;
}

int EchoPubSubCommandResponse::binarySize() const {
	int total = sizeof(this->type);

	total += PubsubUtils::stringSize(this->message);

	return total;
}

void EchoPubSubCommandResponse::toBinary(ByteBuffer *buff) const {
	buff->putInt(this->type);

	PubsubUtils::putString(buff, this->message);
}

void EchoPubSubCommandResponse::fromBinary(ByteBuffer *buff) {
	UnicodeString* msg = PubsubUtils::getString(buff);
	setMessage(msg);
}

void EchoPubSubCommandResponse::setMessage(UnicodeString* str) noexcept {
	delete this->message;
	this->message = str;
}

UnicodeString* EchoPubSubCommandResponse::toString() const noexcept {
	UnicodeString* str = new UnicodeString(L"[echo command] ");
	str->append(this->message);

	return str;
}

} /* namespace codablecash */
