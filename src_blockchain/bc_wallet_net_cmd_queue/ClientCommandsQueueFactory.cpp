/*
 * ClientCommandsQueueFactory.cpp
 *
 *  Created on: Apr 15, 2025
 *      Author: iizuka
 */

#include "bc_wallet_net_cmd_queue/ClientCommandsQueueFactory.h"

#include "bc_wallet_net_cmd/AbstractClientCommand.h"

namespace codablecash {

ClientCommandsQueueFactory::ClientCommandsQueueFactory() {

}

ClientCommandsQueueFactory::~ClientCommandsQueueFactory() {

}

IBlockObject* ClientCommandsQueueFactory::makeDataFromBinary(ByteBuffer *in) {
	return AbstractClientCommand::createFromBinary(in);
}

AbstractBtreeDataFactory* ClientCommandsQueueFactory::copy() const noexcept {
	return new ClientCommandsQueueFactory();
}

} /* namespace codablecash */
