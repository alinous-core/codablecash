/*
 * EchoPubSubCommand.cpp
 *
 *  Created on: 2023/01/12
 *      Author: iizuka
 */

#include "pubsub_cmd/EchoPubSubCommand.h"
#include "pubsub_cmd/EchoPubSubCommandResponse.h"

#include "base_io/ByteBuffer.h"

#include "base/UnicodeString.h"

#include "pubsub/IPubsubCommandListner.h"

namespace codablecash {

EchoPubSubCommand::EchoPubSubCommand(const EchoPubSubCommand &inst) : AbstractPubSubCommand(AbstractPubSubCommand::TYPE_ECHO) {
	this->message = inst.message != nullptr ? new UnicodeString(inst.message) : nullptr;
}

EchoPubSubCommand::EchoPubSubCommand() : AbstractPubSubCommand(AbstractPubSubCommand::TYPE_ECHO) {
	this->message = nullptr;
}

EchoPubSubCommand::~EchoPubSubCommand() {
	delete this->message;
}

int EchoPubSubCommand::binarySize() const {
	int total = sizeof(this->type);

	total += stringSize(this->message);

	return total;
}

void EchoPubSubCommand::toBinary(ByteBuffer *buff) const {
	buff->putInt(this->type);

	putString(buff, this->message);
}

void EchoPubSubCommand::fromBinary(ByteBuffer *buff) {
	UnicodeString* msg = getString(buff);
	setMessage(msg);
}

void EchoPubSubCommand::setMessage(UnicodeString* str) noexcept {
	delete this->message;
	this->message = str;
}

AbstractCommandResponse* EchoPubSubCommand::execute(const PubSubId* pubsubId, IPubsubCommandListner *listner) const {
	listner->fireExecuteCommand(this);

	EchoPubSubCommandResponse* res = new EchoPubSubCommandResponse();
	res->setMessage(new UnicodeString(*this->message));

	return res;
}

void EchoPubSubCommand::setMessage(const wchar_t *msg) noexcept {
	UnicodeString* str = new UnicodeString(msg);
	setMessage(str);
}

IBlockObject* EchoPubSubCommand::copyData() const noexcept {
	return new EchoPubSubCommand(*this);
}

} /* namespace codablecash */
