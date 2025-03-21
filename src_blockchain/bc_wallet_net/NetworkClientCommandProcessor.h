/*
 * NetworkClientCommandProcessor.h
 *
 *  Created on: Mar 21, 2025
 *      Author: iizuka
 */

#ifndef BC_WALLET_NET_NETWORKCLIENTCOMMANDPROCESSOR_H_
#define BC_WALLET_NET_NETWORKCLIENTCOMMANDPROCESSOR_H_

#include "pubsub/IPubsubCommandListner.h"

namespace codablecash {

class NetworkWallet;
class ClientExecutor;

class NetworkClientCommandProcessor : public IPubsubCommandListner {
public:
	explicit NetworkClientCommandProcessor(NetworkWallet* networkWallet);
	virtual ~NetworkClientCommandProcessor();

	virtual void fireExecuteCommand(const AbstractPubSubCommand* cmd);
	virtual void fireOnExeptionOnSubscriber(const Exception* e);
	virtual void fireOnSubscribeEnds();

	virtual IPubsubCommandExecutor* getExecutor() const noexcept;

private:
	NetworkWallet* networkWallet;

	ClientExecutor* clientExec;
};

} /* namespace codablecash */

#endif /* BC_WALLET_NET_NETWORKCLIENTCOMMANDPROCESSOR_H_ */
