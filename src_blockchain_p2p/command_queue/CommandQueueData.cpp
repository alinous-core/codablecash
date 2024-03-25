/*
 * CommandQueueData.cpp
 *
 *  Created on: 2023/09/25
 *      Author: iizuka
 */

#include "command_queue/CommandQueueData.h"

#include "command_queue_cmd/AbstructNodeQueueCommand.h"

#include "pubsub/PubSubId.h"

#include "bc_base/BinaryUtils.h"

#include "base/StackRelease.h"


namespace codablecash {

CommandQueueData::CommandQueueData(const CommandQueueData &inst) {
	this->pubsubId = inst.pubsubId != nullptr ? new PubSubId(*inst.pubsubId) : nullptr;
	this->command = inst.command != nullptr ? dynamic_cast<AbstructNodeQueueCommand*>(inst.command->copyData()) : nullptr;
}

CommandQueueData::CommandQueueData() {
	this->pubsubId = nullptr;
	this->command = nullptr;
}

CommandQueueData::~CommandQueueData() {
	delete this->pubsubId;
	delete this->command;
}

int CommandQueueData::binarySize() const {
	BinaryUtils::checkNotNull(this->pubsubId);
	BinaryUtils::checkNotNull(this->command);

	int total = this->pubsubId->binarySize();
	total += this->command->binarySize();

	return total;
}

void CommandQueueData::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->pubsubId);
	BinaryUtils::checkNotNull(this->command);

	this->pubsubId->toBinary(out);
	this->command->toBinary(out);
}

CommandQueueData* CommandQueueData::createFromBinary(ByteBuffer *in) {
	CommandQueueData* data = new CommandQueueData(); __STP(data);

	data->pubsubId = PubSubId::fromBinary(in);
	data->command = AbstructNodeQueueCommand::createFromBinary(in);

	BinaryUtils::checkNotNull(data->pubsubId);
	BinaryUtils::checkNotNull(data->command);

	return __STP_MV(data);
}

IBlockObject* CommandQueueData::copyData() const noexcept {
	return new CommandQueueData(*this);
}

void CommandQueueData::setPublishId(const PubSubId *pubsubId) noexcept {
	delete this->pubsubId;
	this->pubsubId = new PubSubId(*pubsubId);
}

void CommandQueueData::setCommand(const AbstructNodeQueueCommand *command) noexcept {
	delete this->command;
	this->command = dynamic_cast<AbstructNodeQueueCommand*>(command->copyData());
}

} /* namespace codablecash */
