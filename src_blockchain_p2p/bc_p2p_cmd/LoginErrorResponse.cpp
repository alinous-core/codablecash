/*
 * LoginErrorResponse.cpp
 *
 *  Created on: 2023/09/09
 *      Author: iizuka
 */

#include "bc_p2p_cmd/LoginErrorResponse.h"

#include "base_io/ByteBuffer.h"

#include "base/UnicodeString.h"

namespace codablecash {

LoginErrorResponse::LoginErrorResponse() : AbstractCommandResponse(TYPE_RES_LOGIN_ERROR) {

}

LoginErrorResponse::~LoginErrorResponse() {

}

int LoginErrorResponse::binarySize() const {
	int total = sizeof(this->type);

	return total;
}

void LoginErrorResponse::toBinary(ByteBuffer *buff) const {
	buff->putInt(this->type);
}

void LoginErrorResponse::fromBinary(ByteBuffer *buff) {
}

UnicodeString* LoginErrorResponse::toString() const noexcept {
	UnicodeString* str = new UnicodeString(L"Login Error");

	return str;
}

} /* namespace codablecash */
