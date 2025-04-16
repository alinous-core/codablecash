/*
 * ClientCommandsQueueDataFactory.cpp
 *
 *  Created on: Mar 26, 2025
 *      Author: iizuka
 */

#include "bc_wallet_net_cmd_queue/ClientCommandsQueueDataFactory.h"
#include "bc_wallet_net_cmd_queue/ClientCommandsQueueData.h"


namespace codablecash {

ClientCommandsQueueDataFactory::ClientCommandsQueueDataFactory() {

}

ClientCommandsQueueDataFactory::~ClientCommandsQueueDataFactory() {

}

IBlockObject* ClientCommandsQueueDataFactory::makeDataFromBinary(ByteBuffer *in) {
	return ClientCommandsQueueData::createFromBinary(in);
}

AbstractBtreeDataFactory* ClientCommandsQueueDataFactory::copy() const noexcept {
	return new ClientCommandsQueueDataFactory();
}

} /* namespace codablecash */
