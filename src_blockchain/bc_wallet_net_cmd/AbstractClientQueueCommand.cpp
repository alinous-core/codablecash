/*
 * AbstractClientCommand.cpp
 *
 *  Created on: Apr 15, 2025
 *      Author: iizuka
 */

#include "bc_wallet_net_cmd/AbstractClientQueueCommand.h"
#include "bc_wallet_net_cmd/ClientNewTransactionCommand.h"


namespace codablecash {

AbstractClientQueueCommand::AbstractClientQueueCommand(uint8_t type) {
	this->type = type;
}

AbstractClientQueueCommand::~AbstractClientQueueCommand() {

}

AbstractClientQueueCommand::AbstractClientQueueCommand(const AbstractClientQueueCommand &inst) {
	this->type = inst.type;
}

int AbstractClientQueueCommand::binarySize() const {
	return sizeof(this->type);
}

void AbstractClientQueueCommand::toBinary(ByteBuffer *out) const {
	out->put(this->type);
}

AbstractClientQueueCommand* AbstractClientQueueCommand::createFromBinary(ByteBuffer *in) {
	uint8_t t = in->get();

	AbstractClientQueueCommand* cmd = nullptr;
	switch(t){
	case CLIENT_NEW_TRANSACTION:
		cmd = new ClientNewTransactionCommand();
		break;
	default:
		return nullptr;
	}

	cmd->fromBinary(in);

	return cmd;
}

} /* namespace codablecash */
