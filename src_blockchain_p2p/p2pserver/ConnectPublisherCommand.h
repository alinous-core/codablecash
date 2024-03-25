/*
 * ConnectPublisherCommand.h
 *
 *  Created on: 2023/01/10
 *      Author: iizuka
 */

#ifndef P2PSERVER_CONNECTPUBLISHERCOMMAND_H_
#define P2PSERVER_CONNECTPUBLISHERCOMMAND_H_

#include "p2pserver/AbstractConnectionCommand.h"

namespace codablecash {

class PubSubId;

class ConnectPublisherCommand : public AbstractConnectionCommand {
public:
	ConnectPublisherCommand();
	explicit ConnectPublisherCommand(const PubSubId* pubsubId);
	virtual ~ConnectPublisherCommand();

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

#endif /* P2PSERVER_CONNECTPUBLISHERCOMMAND_H_ */
