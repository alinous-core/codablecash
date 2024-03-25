/*
 * CommandQueueDataFactory.cpp
 *
 *  Created on: 2023/09/19
 *      Author: iizuka
 */

#include "command_queue/CommandQueueDataFactory.h"
#include "command_queue/CommandQueueData.h"

namespace codablecash {

CommandQueueDataFactory::CommandQueueDataFactory() {

}

CommandQueueDataFactory::~CommandQueueDataFactory() {

}

IBlockObject* CommandQueueDataFactory::makeDataFromBinary(ByteBuffer *in) {
	return CommandQueueData::createFromBinary(in);
}

AbstractBtreeDataFactory* CommandQueueDataFactory::copy() const noexcept {
	return new CommandQueueDataFactory();
}

} /* namespace codablecash */
