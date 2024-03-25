/*
 * NodeTransactionAcceptionQueueCommand.cpp
 *
 *  Created on: 2023/10/02
 *      Author: iizuka
 */

#include "command_queue_cmd/NodeTransactionAcceptionQueueCommand.h"

namespace codablecash {

NodeTransactionAcceptionQueueCommand::NodeTransactionAcceptionQueueCommand(const NodeTransactionAcceptionQueueCommand &inst)
		: AbstractTransactionAcceptionQueueCommand(inst) {
}

NodeTransactionAcceptionQueueCommand::NodeTransactionAcceptionQueueCommand()
	: AbstractTransactionAcceptionQueueCommand(TYPE_NODE_TRANSACTION_ACCEPT_QUEUE) {

}

NodeTransactionAcceptionQueueCommand::~NodeTransactionAcceptionQueueCommand() {

}

IBlockObject* NodeTransactionAcceptionQueueCommand::copyData() const noexcept {
	return new NodeTransactionAcceptionQueueCommand(*this);
}

} /* namespace codablecash */
