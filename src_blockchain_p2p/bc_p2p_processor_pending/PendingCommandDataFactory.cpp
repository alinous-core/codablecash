/*
 * AbstructNodeQueueCommandDataFactory.cpp
 *
 *  Created on: 2023/11/11
 *      Author: iizuka
 */
#include "bc_p2p_processor_pending/PendingCommandDataFactory.h"

#include "pubsub_cmd/AbstractPubSubCommand.h"

#include "bc_p2p_cmd_node/AbstractNodeCommand.h"

#include "bc_p2p_processor_pending/PendingCommandData.h"

#include "base/StackRelease.h"

#include "bc_base/BinaryUtils.h"


namespace codablecash {

PendingCommandDataFactory::PendingCommandDataFactory() {

}

PendingCommandDataFactory::~PendingCommandDataFactory() {

}

IBlockObject* PendingCommandDataFactory::makeDataFromBinary(ByteBuffer *in) {
	return PendingCommandData::createFromBinary(in);
}

AbstractBtreeDataFactory* PendingCommandDataFactory::copy() const noexcept {
	return new PendingCommandDataFactory();
}

} /* namespace codablecash */
