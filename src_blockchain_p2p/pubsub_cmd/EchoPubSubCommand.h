/*
 * EchoPubSubCommand.h
 *
 *  Created on: 2023/01/12
 *      Author: iizuka
 */

#ifndef PUBSUB_ECHOPUBSUBCOMMAND_H_
#define PUBSUB_ECHOPUBSUBCOMMAND_H_

namespace alinous {
class UnicodeString;
}
using namespace alinous;

#include "pubsub_cmd/AbstractPubSubCommand.h"

namespace codablecash {

class EchoPubSubCommand : public AbstractPubSubCommand {
public:
	EchoPubSubCommand(const EchoPubSubCommand& inst);
	EchoPubSubCommand();
	virtual ~EchoPubSubCommand();

	virtual void fromBinary(ByteBuffer* buff);
	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* buff) const;

	virtual AbstractCommandResponse* execute(const PubSubId* pubsubId, IPubsubCommandListner* listner) const;

	void setMessage(UnicodeString* str) noexcept;
	void setMessage(const wchar_t* msg) noexcept;

	virtual IBlockObject* copyData() const noexcept;

private:
	UnicodeString* message;
};

} /* namespace codablecash */

#endif /* PUBSUB_ECHOPUBSUBCOMMAND_H_ */
