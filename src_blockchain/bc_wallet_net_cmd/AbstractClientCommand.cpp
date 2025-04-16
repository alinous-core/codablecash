/*
 * AbstractClientCommand.cpp
 *
 *  Created on: Apr 15, 2025
 *      Author: iizuka
 */

#include "bc_wallet_net_cmd/AbstractClientCommand.h"
#include "bc_wallet_net_cmd/ClientNewTransactionCommand.h"


namespace codablecash {

AbstractClientCommand::AbstractClientCommand(uint8_t type) {
	this->type = type;
}

AbstractClientCommand::~AbstractClientCommand() {

}

AbstractClientCommand::AbstractClientCommand(const AbstractClientCommand &inst) {
	this->type = inst.type;
}

int AbstractClientCommand::binarySize() const {
	return sizeof(this->type);
}

void AbstractClientCommand::toBinary(ByteBuffer *out) const {
	out->put(this->type);
}

AbstractClientCommand* AbstractClientCommand::createFromBinary(ByteBuffer *in) {
	uint8_t t = in->get();

	AbstractClientCommand* cmd = nullptr;
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
