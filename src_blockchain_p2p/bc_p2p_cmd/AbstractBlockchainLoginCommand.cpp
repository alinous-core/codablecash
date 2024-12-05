/*
 * AbstractBlockchainLoginCommand.cpp
 *
 *  Created on: 2023/09/07
 *      Author: iizuka
 */

#include "bc_p2p_cmd/AbstractBlockchainLoginCommand.h"

#include "base_io/ByteBuffer.h"

#include "bc_base/BinaryUtils.h"

namespace codablecash {

AbstractBlockchainLoginCommand::AbstractBlockchainLoginCommand(const AbstractBlockchainLoginCommand &inst)
		: AbstractBlockchainPubsubCommand(inst) {
	this->zone = inst.zone;
	this->canonicalName = inst.canonicalName != nullptr ? new UnicodeString(inst.canonicalName) : nullptr;
}

AbstractBlockchainLoginCommand::AbstractBlockchainLoginCommand(uint32_t type, uint16_t zone, const UnicodeString* canonicalName)
		: AbstractBlockchainPubsubCommand(type) {
	this->zone = zone;
	this->canonicalName = canonicalName != nullptr ? new UnicodeString(canonicalName) : nullptr;
}

AbstractBlockchainLoginCommand::~AbstractBlockchainLoginCommand() {
	delete this->canonicalName;
}

int AbstractBlockchainLoginCommand::binarySize() const {
	int total = AbstractBlockchainPubsubCommand::binarySize();

	total += sizeof(this->zone);

	total += sizeof(uint8_t);
	if(this->canonicalName != nullptr){
		total += BinaryUtils::stringSize(this->canonicalName);
	}

	return total;
}

void AbstractBlockchainLoginCommand::toBinary(ByteBuffer *buff) const {
	AbstractBlockchainPubsubCommand::toBinary(buff);

	buff->putShort(this->zone);

	bool bl = (this->canonicalName);
	buff->put(bl ? 1 : 0);
	if(bl){
		BinaryUtils::putString(buff, this->canonicalName);
	}
}

void AbstractBlockchainLoginCommand::fromBinary(ByteBuffer *buff) {
	AbstractBlockchainPubsubCommand::fromBinary(buff);

	this->zone = buff->getShort();

	uint8_t bl = buff->get();
	if(bl > 0){
		this->canonicalName = BinaryUtils::getString(buff);
	}
}

} /* namespace codablecash */
