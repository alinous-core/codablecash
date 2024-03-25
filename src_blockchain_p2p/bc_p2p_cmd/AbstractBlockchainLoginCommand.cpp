/*
 * AbstractBlockchainLoginCommand.cpp
 *
 *  Created on: 2023/09/07
 *      Author: iizuka
 */

#include "bc_p2p_cmd/AbstractBlockchainLoginCommand.h"

#include "base_io/ByteBuffer.h"

namespace codablecash {

AbstractBlockchainLoginCommand::AbstractBlockchainLoginCommand(const AbstractBlockchainLoginCommand &inst)
		: AbstractBlockchainPubsubCommand(inst) {
	this->zone = inst.zone;
}

AbstractBlockchainLoginCommand::AbstractBlockchainLoginCommand(uint32_t type, uint16_t zone)
		: AbstractBlockchainPubsubCommand(type) {
	this->zone = zone;
}

AbstractBlockchainLoginCommand::~AbstractBlockchainLoginCommand() {

}

int AbstractBlockchainLoginCommand::binarySize() const {
	int total = AbstractBlockchainPubsubCommand::binarySize();

	total += sizeof(this->zone);

	return total;
}

void AbstractBlockchainLoginCommand::toBinary(ByteBuffer *buff) const {
	AbstractBlockchainPubsubCommand::toBinary(buff);

	buff->putShort(this->zone);
}

void AbstractBlockchainLoginCommand::fromBinary(ByteBuffer *buff) {
	AbstractBlockchainPubsubCommand::fromBinary(buff);

	this->zone = buff->getShort();
}

} /* namespace codablecash */
