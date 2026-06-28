/*
 * NewShardZoneCommand.cpp
 *
 *  Created on: Jun 13, 2026
 *      Author: iizuka
 */

#include "bc_block_header_command/NewShardZoneCommand.h"

namespace codablecash {


NewShardZoneCommand::NewShardZoneCommand(const NewShardZoneCommand &inst) : AbstractBlockHeaderCommand(inst) {
	this->newShardNo = inst.newShardNo;
}

NewShardZoneCommand::NewShardZoneCommand() : AbstractBlockHeaderCommand(AbstractBlockHeaderCommand::NEW_SHARD_COMMAND) {
	this->newShardNo = 0;
}

NewShardZoneCommand::~NewShardZoneCommand() {

}

int NewShardZoneCommand::binarySize() const {
	int total = sizeof(uint16_t);

	total += sizeof(uint16_t);

	return total;
}

void NewShardZoneCommand::toBinary(ByteBuffer *out) const {
	out->putShort(this->type);

	out->putShort(this->newShardNo);
}

void NewShardZoneCommand::fromBinary(ByteBuffer* in) {
	this->newShardNo = in->getShort();
}

IBlockObject* NewShardZoneCommand::copyData() const noexcept {
	return new NewShardZoneCommand(*this);
}

void NewShardZoneCommand::setNewShardNo(uint16_t newShardNo) noexcept {
	this->newShardNo = newShardNo;
}

} /* namespace codablecash */
