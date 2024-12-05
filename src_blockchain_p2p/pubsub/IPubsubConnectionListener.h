/*
 * IPubsubConnectionListener.h
 *
 *  Created on: 2023/04/05
 *      Author: iizuka
 */

#ifndef PUBSUB_IPUBSUBCONNECTIONLISTENER_H_
#define PUBSUB_IPUBSUBCONNECTIONLISTENER_H_

namespace alinous {
class UnicodeString;
}
using namespace alinous;

namespace codablecash {

class P2pHandshake;
class LoginPubSubCommand;
class LoginClientCommand;

class IPubsubConnectionListener {
public:
	IPubsubConnectionListener();
	virtual ~IPubsubConnectionListener();

	virtual void onLoginHandshake(P2pHandshake *handshake,	const LoginPubSubCommand *loginCommand, const UnicodeString* canonicalName) = 0;
	virtual void onLoginClinentHandshake(P2pHandshake *handshake,	const LoginClientCommand *clientLoginCommand) = 0;
	virtual void onHandshakeEnds(P2pHandshake *handshake) = 0;
};

} /* namespace codablecash */

#endif /* PUBSUB_IPUBSUBCONNECTIONLISTENER_H_ */
