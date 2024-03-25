/*
 * SignatureErrorResponse.cpp
 *
 *  Created on: 2023/04/06
 *      Author: iizuka
 */

#include "bc_p2p_cmd/SignatureErrorResponse.h"

#include "base_io/ByteBuffer.h"

#include "base/UnicodeString.h"
namespace codablecash {

SignatureErrorResponse::SignatureErrorResponse() : AbstractCommandResponse(AbstractCommandResponse::TYPE_RES_SIGNATURE_ERROR) {

}

SignatureErrorResponse::~SignatureErrorResponse() {

}

void SignatureErrorResponse::fromBinary(ByteBuffer *buff) {
}

int SignatureErrorResponse::binarySize() const {
	int total = sizeof(this->type);

	return total;
}

void SignatureErrorResponse::toBinary(ByteBuffer *buff) const {
	buff->putInt(this->type);
}

UnicodeString* SignatureErrorResponse::toString() const noexcept {
	UnicodeString* str = new UnicodeString(L"[Command Response]Signature Error");

	return str;
}

} /* namespace codablecash */
