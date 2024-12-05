/*
 * NodeCommandValidationErrorResponse.cpp
 *
 *  Created on: Aug 21, 2024
 *      Author: iizuka
 */

#include "bc_p2p_cmd_node/NodeCommandValidationErrorResponse.h"

#include "base/UnicodeString.h"

#include "base_io/ByteBuffer.h"

namespace codablecash {


NodeCommandValidationErrorResponse::NodeCommandValidationErrorResponse() : AbstractCommandResponse(AbstractCommandResponse::TYPE_NODE_COMMAND_VALIDATION_ERROR) {

}

NodeCommandValidationErrorResponse::~NodeCommandValidationErrorResponse() {

}

int NodeCommandValidationErrorResponse::binarySize() const {
	int total = sizeof(this->type);

	return total;
}

void NodeCommandValidationErrorResponse::toBinary(ByteBuffer *buff) const {
	buff->putInt(this->type);
}

void NodeCommandValidationErrorResponse::fromBinary(ByteBuffer *buff) {
}

UnicodeString* NodeCommandValidationErrorResponse::toString() const noexcept {
	UnicodeString* str = new UnicodeString(L"VALIDATEION ERROR");

	return str;
}

} /* namespace codablecash */
