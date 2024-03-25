/*
 * ConnectSubscriberCommand.h
 *
 *  Created on: 2023/01/10
 *      Author: iizuka
 */

#ifndef P2PSERVER_CONNECTSUBSCRIBERCOMMAND_H_
#define P2PSERVER_CONNECTSUBSCRIBERCOMMAND_H_

#include "p2pserver/AbstractConnectionCommand.h"

namespace codablecash {

class PubSubId;

class ConnectSubscriberCommand : public AbstractConnectionCommand {
public:
	ConnectSubscriberCommand();
	explicit ConnectSubscriberCommand(const PubSubId* pubsubId);
	virtual ~ConnectSubscriberCommand();

	virtual void fromBinary(ByteBuffer* buff);
	virtual int binarySize();
	virtual void toBinary(ByteBuffer* buff);

	virtual void process(P2pConnectionAcceptThread* acceptThread);

	const PubSubId* getPubsubId() const noexcept {
		return pubsubId;
	}
private:
	PubSubId* pubsubId;
};

} /* namespace codablecash */

#endif /* P2PSERVER_CONNECTSUBSCRIBERCOMMAND_H_ */
