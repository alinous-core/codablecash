/*
 * PingNodeCommand.cpp
 *
 *  Created on: 2023/09/10
 *      Author: iizuka
 */

#include "bc_p2p_cmd_client/PingNodeCommand.h"
#include "bc_p2p_cmd_client/PingNodeCommandResponse.h"

#include "base_io/ByteBuffer.h"

namespace codablecash {

PingNodeCommand::PingNodeCommand(const PingNodeCommand &inst) : AbstractClientRequestCommand(inst) {
}

PingNodeCommand::PingNodeCommand() : AbstractClientRequestCommand(TYPE_CLIENT_PING) {

}

PingNodeCommand::~PingNodeCommand() {

}

AbstractCommandResponse* PingNodeCommand::executeAsClient(ClientNodeHandshake *clientHandshake, CodablecashNodeInstance *inst) const {
	return new PingNodeCommandResponse();
}

IBlockObject* PingNodeCommand::copyData() const noexcept {
	return new PingNodeCommand(*this);
}

ByteBuffer* PingNodeCommand::getSignBinary() const {
	const char* pingstr = "ping";

	return ByteBuffer::wrapWithEndian(pingstr, 5, true);
}

} /* namespace codablecash */
