/*
 * PingNodeCommandResponse.cpp
 *
 *  Created on: 2023/09/10
 *      Author: iizuka
 */

#include "bc_p2p_cmd_client/PingNodeCommandResponse.h"

#include "base_io/ByteBuffer.h"

#include "base/UnicodeString.h"

namespace codablecash {

PingNodeCommandResponse::PingNodeCommandResponse() : AbstractCommandResponse(TYPE_RES_PING) {

}

PingNodeCommandResponse::~PingNodeCommandResponse() {

}

int PingNodeCommandResponse::binarySize() const {
	int total = sizeof(this->type);

	return total;
}

void PingNodeCommandResponse::toBinary(ByteBuffer *buff) const {
	buff->putInt(this->type);
}

void PingNodeCommandResponse::fromBinary(ByteBuffer *buff) {
}

UnicodeString* PingNodeCommandResponse::toString() const noexcept {
	UnicodeString* str = new UnicodeString(L"Pong");

	return str;
}

} /* namespace codablecash */
