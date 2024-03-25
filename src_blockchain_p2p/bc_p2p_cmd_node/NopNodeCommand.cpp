/*
 * NopNodeCommand.cpp
 *
 *  Created on: 2023/12/04
 *      Author: iizuka
 */

#include "bc_p2p_cmd_node/NopNodeCommand.h"
#include "bc_p2p_cmd_node/InvalidTransactionException.h"

#include "pubsub_cmd/OkPubsubResponse.h"


namespace codablecash {

NopNodeCommand::NopNodeCommand(const NopNodeCommand &inst) : AbstractNodeCommand(inst) {
	this->value = inst.value;
}

NopNodeCommand::NopNodeCommand() : AbstractNodeCommand(TYPE_NODE_NOP) {
	this->value = 0;
}

NopNodeCommand::~NopNodeCommand() {

}

int NopNodeCommand::binarySize() const {
	int total = AbstractNodeCommand::binarySize();

	total += sizeof(this->value);

	return total;
}

void NopNodeCommand::toBinary(ByteBuffer *buff) const {
	AbstractNodeCommand::toBinary(buff);

	buff->put(this->value);
}

ByteBuffer* NopNodeCommand::getSignBinary() const {
	int total = sizeof(this->value);

	ByteBuffer* buff = ByteBuffer::allocateWithEndian(total, true);
	buff->put(this->value);

	return buff;
}

void NopNodeCommand::fromBinary(ByteBuffer *buff) {
	AbstractNodeCommand::fromBinary(buff);

	this->value = buff->get();
}

IBlockObject* NopNodeCommand::copyData() const noexcept {
	return new NopNodeCommand(*this);
}

AbstractCommandResponse* NopNodeCommand::executeAsNode(BlockchainNodeHandshake *nodeHandShake, CodablecashNodeInstance *inst,	bool suspend) const {
	/*if(this->value == 1){
		throw new InvalidTransactionException(__FILE__, __LINE__);
	}*/
	return new OkPubsubResponse();
}

void NopNodeCommand::setValue(uint8_t v) noexcept {
	this->value = v;
}

} /* namespace codablecash */
