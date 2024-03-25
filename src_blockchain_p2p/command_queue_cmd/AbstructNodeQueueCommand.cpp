/*
 * AbstructNodeQueueCommand.cpp
 *
 *  Created on: 2023/09/29
 *      Author: iizuka
 */

#include "command_queue_cmd/AbstructNodeQueueCommand.h"
#include "command_queue_cmd/NodeQueueCommandException.h"
#include "command_queue_cmd/ClientTransactionAcceptionQueueCommand.h"
#include "command_queue_cmd/NodeTransactionAcceptionQueueCommand.h"

#include "base_io/ByteBuffer.h"
#include "command_queue_cmd/NopNodeQueueCommand.h"


namespace codablecash {

AbstructNodeQueueCommand::AbstructNodeQueueCommand(const AbstructNodeQueueCommand &inst) {
	this->type = inst.type;
}

AbstructNodeQueueCommand::AbstructNodeQueueCommand(uint32_t type) {
	this->type = type;
}

AbstructNodeQueueCommand::~AbstructNodeQueueCommand() {

}

AbstructNodeQueueCommand* AbstructNodeQueueCommand::createFromBinary(ByteBuffer *in) {
	AbstructNodeQueueCommand* ret = nullptr;

	uint32_t type = in->getInt();
	switch(type){
	case TYPE_NODE_NOP:
		ret = new NopNodeQueueCommand();
		break;
	case TYPE_NODE_CLIENT_TRANSACTION_ACCEPT_QUEUE:
		ret = new ClientTransactionAcceptionQueueCommand();
		break;
	case TYPE_NODE_TRANSACTION_ACCEPT_QUEUE:
		ret = new NodeTransactionAcceptionQueueCommand();
		break;
	default:
		throw new NodeQueueCommandException(L"Wrong command type.", __FILE__, __LINE__);
	}

	ret->fromBinary(in);

	return ret;
}

} /* namespace codablecash */
