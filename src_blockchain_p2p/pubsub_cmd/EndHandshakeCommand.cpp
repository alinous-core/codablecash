/*
 * EndHandshakeCommand.cpp
 *
 *  Created on: 2023/01/15
 *      Author: iizuka
 */

#include "pubsub_cmd/EndHandshakeCommand.h"
#include "pubsub_cmd/OkPubsubResponse.h"
#include "pubsub/IPubsubCommandListner.h"

#include "base_io/ByteBuffer.h"



namespace codablecash {

EndHandshakeCommand::EndHandshakeCommand(const EndHandshakeCommand &inst) : AbstractPubSubCommand(AbstractPubSubCommand::TYPE_END_HANDSHAKE) {

}

EndHandshakeCommand::EndHandshakeCommand() : AbstractPubSubCommand(AbstractPubSubCommand::TYPE_END_HANDSHAKE) {

}

EndHandshakeCommand::~EndHandshakeCommand() {

}

void EndHandshakeCommand::fromBinary(ByteBuffer *buff) {
}

int EndHandshakeCommand::binarySize() const {
	int total = sizeof(this->type);

	return total;
}

void EndHandshakeCommand::toBinary(ByteBuffer *buff) const {
	buff->putInt(this->type);
}

AbstractCommandResponse* EndHandshakeCommand::execute(const PubSubId* pubsubId, IPubsubCommandListner *listner) const {
	listner->fireExecuteCommand(this);

	return new OkPubsubResponse();
}

IBlockObject* EndHandshakeCommand::copyData() const noexcept {
	return new EndHandshakeCommand(*this);
}

} /* namespace codablecash */
