/*
 * NetworkClientCommandListner.h
 *
 *  Created on: Mar 21, 2025
 *      Author: iizuka
 */

#ifndef BC_WALLET_NET_NETWORKCLIENTCOMMANDLISTNER_H_
#define BC_WALLET_NET_NETWORKCLIENTCOMMANDLISTNER_H_

#include "bc_p2p_cmd_client_notify/IClientNotifyListner.h"

namespace codablecash {

class NetworkClientCommandProcessor;

class NetworkClientCommandListner : public IClientNotifyListner {
public:
	explicit NetworkClientCommandListner(NetworkClientCommandProcessor* processor);
	virtual ~NetworkClientCommandListner();

	virtual void onNewTransaction(const PubSubId *pubsubId, const TransactionTransferData* data);

private:
	NetworkClientCommandProcessor* processor;
};

} /* namespace codablecash */

#endif /* BC_WALLET_NET_NETWORKCLIENTCOMMANDLISTNER_H_ */
