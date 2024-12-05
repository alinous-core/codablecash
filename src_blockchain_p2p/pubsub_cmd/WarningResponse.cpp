/*
 * WarningResponse.cpp
 *
 *  Created on: Sep 23, 2024
 *      Author: iizuka
 */

#include "pubsub_cmd/WarningResponse.h"

#include "base/UnicodeString.h"

#include "base_io/ByteBuffer.h"

#include "bc_base/BinaryUtils.h"


namespace codablecash {

WarningResponse::WarningResponse() : AbstractCommandResponse(AbstractCommandResponse::TYPE_RES_WARNING) {
	this->message = nullptr;
}

WarningResponse::~WarningResponse() {
	delete this->message;
}

int WarningResponse::binarySize() const {
	BinaryUtils::checkNotNull(this->message);

	int total = sizeof(this->type);
	total += BinaryUtils::stringSize(this->message);

	return total;
}

void WarningResponse::toBinary(ByteBuffer *buff) const {
	BinaryUtils::checkNotNull(this->message);

	buff->putInt(this->type);
	BinaryUtils::putString(buff, this->message);
}

void WarningResponse::fromBinary(ByteBuffer *buff) {
	this->message = BinaryUtils::getString(buff);

	BinaryUtils::checkNotNull(this->message);
}

UnicodeString* WarningResponse::toString() const noexcept {
	UnicodeString* str = new UnicodeString(L"Warning: ");
	str->append(this->message);

	return str;
}

void WarningResponse::setMessage(const UnicodeString *str) noexcept {
	delete this->message;
	this->message = new UnicodeString(str);
}

}
