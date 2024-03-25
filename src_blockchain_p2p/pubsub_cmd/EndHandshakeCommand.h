/*
 * EndHandshakeCommand.h
 *
 *  Created on: 2023/01/15
 *      Author: iizuka
 */

#ifndef PUBSUB_ENDHANDSHAKECOMMAND_H_
#define PUBSUB_ENDHANDSHAKECOMMAND_H_

#include "pubsub_cmd/AbstractPubSubCommand.h"

namespace codablecash {

class EndHandshakeCommand : public AbstractPubSubCommand {
public:
	EndHandshakeCommand(const EndHandshakeCommand& inst);
	EndHandshakeCommand();
	virtual ~EndHandshakeCommand();

	virtual void fromBinary(ByteBuffer* buff);
	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* buff) const;

	virtual AbstractCommandResponse* execute(const PubSubId* pubsubId, IPubsubCommandListner* listner) const;

	virtual IBlockObject* copyData() const noexcept;
};

} /* namespace codablecash */

#endif /* PUBSUB_ENDHANDSHAKECOMMAND_H_ */
