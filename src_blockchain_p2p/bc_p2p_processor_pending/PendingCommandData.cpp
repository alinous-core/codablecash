/*
 * PendingCommandData.cpp
 *
 *  Created on: 2023/11/11
 *      Author: iizuka
 */

#include "bc_p2p_processor_pending/PendingCommandData.h"

#include "bc_base/BinaryUtils.h"

#include "pubsub/PubSubId.h"

#include "base/StackRelease.h"

#include "bc_p2p_cmd_node/AbstractNodeCommand.h"

namespace codablecash {

PendingCommandData::PendingCommandData(const PendingCommandData &inst) {
	this->pubsubId = inst.pubsubId != nullptr ? new PubSubId(*inst.pubsubId) : nullptr;
	this->command = inst.command != nullptr ? dynamic_cast<AbstractNodeCommand*>(inst.command->copyData()) : nullptr;
}

PendingCommandData::PendingCommandData() {
	this->pubsubId = nullptr;
	this->command = nullptr;
}

PendingCommandData::~PendingCommandData() {
	delete this->pubsubId;
	delete this->command;
}

int PendingCommandData::binarySize() const {
	BinaryUtils::checkNotNull(this->pubsubId);
	BinaryUtils::checkNotNull(this->command);

	int total = this->pubsubId->binarySize();
	total += this->command->binarySize();

	return total;
}

void PendingCommandData::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->pubsubId);
	BinaryUtils::checkNotNull(this->command);

	this->pubsubId->toBinary(out);
	this->command->toBinary(out);
}

PendingCommandData* PendingCommandData::createFromBinary(ByteBuffer *in) {
	PendingCommandData* data = new PendingCommandData(); __STP(data);

	data->pubsubId = PubSubId::fromBinary(in);

	AbstractPubSubCommand* cmd = AbstractPubSubCommand::createFromBinary(in); __STP(cmd);
	data->command = dynamic_cast<AbstractNodeCommand*>(cmd);

	BinaryUtils::checkNotNull(data->pubsubId);
	BinaryUtils::checkNotNull(data->command);
	__STP_MV(cmd);

	return __STP_MV(data);
}

IBlockObject* PendingCommandData::copyData() const noexcept {
	return new PendingCommandData(*this);
}

void PendingCommandData::setPubsubId(const PubSubId *pubsubId) noexcept {
	delete this->pubsubId;
	this->pubsubId = new PubSubId(*pubsubId);
}

void PendingCommandData::setNodeCommand(const AbstractNodeCommand *command) noexcept {
	delete this->command;
	this->command = dynamic_cast<AbstractNodeCommand*>(command->copyData());
}

} /* namespace codablecash */
