/*
 * NopConnectionCommand.cpp
 *
 *  Created on: 2022/08/23
 *      Author: iizuka
 */

#include "p2pserver/NopConnectionCommand.h"

#include "ipconnect/IClientSocket.h"

#include "base_io/ByteBuffer.h"

namespace codablecash {

NopConnectionCommand::NopConnectionCommand() : AbstractConnectionCommand(AbstractConnectionCommand::TYPE_NOP) {

}

NopConnectionCommand::~NopConnectionCommand() {

}

void NopConnectionCommand::fromBinary(ByteBuffer *buff) {
}

int NopConnectionCommand::binarySize() {
	int total = sizeof(uint8_t);

	return total;
}

void NopConnectionCommand::toBinary(ByteBuffer *buff) {
	buff->put(AbstractConnectionCommand::TYPE_NOP);
}

void NopConnectionCommand::process(P2pConnectionAcceptThread *acceptThread) {
}

} /* namespace codablecash */
