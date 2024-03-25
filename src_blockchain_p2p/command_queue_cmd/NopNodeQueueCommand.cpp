/*
 * NopNodeCommand.cpp
 *
 *  Created on: 2023/09/22
 *      Author: iizuka
 */

#include "base_io/ByteBuffer.h"
#include "command_queue_cmd/NopNodeQueueCommand.h"

#include "bc_p2p_cmd_node/InvalidTransactionException.h"

namespace codablecash {

NopNodeQueueCommand::NopNodeQueueCommand(const NopNodeQueueCommand &inst) : AbstructNodeQueueCommand(inst) {
	this->num = inst.num;
}

NopNodeQueueCommand::NopNodeQueueCommand() : AbstructNodeQueueCommand(TYPE_NODE_NOP) {
	this->num = 0;
}

NopNodeQueueCommand::~NopNodeQueueCommand() {

}

void NopNodeQueueCommand::execute(const PubSubId *pubsubId, IPubsubCommandListner *listner) {
	if(this->num < 0){
		throw new InvalidTransactionException(__FILE__, __LINE__);
	}
}

IBlockObject* NopNodeQueueCommand::copyData() const noexcept {
	return new NopNodeQueueCommand(*this);
}

int NopNodeQueueCommand::binarySize() const {
	int total = sizeof(this->type);
	total += sizeof(uint32_t);

	return total;
}

void NopNodeQueueCommand::toBinary(ByteBuffer *buff) const {
	buff->putInt(this->type);

	buff->putInt(this->num);
}

void NopNodeQueueCommand::fromBinary(ByteBuffer *buff) {
	this->num = buff->getInt();
}

} /* namespace codablecash */
