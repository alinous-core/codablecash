/*
 * WalletConnectionManager.h
 *
 *  Created on: Feb 21, 2025
 *      Author: iizuka
 */

#ifndef BC_WALLET_NET_WALLETCONNECTIONMANAGER_H_
#define BC_WALLET_NET_WALLETCONNECTIONMANAGER_H_
#include "base/HashMap.h"



using namespace alinous;

namespace codablecash {

class PubSubId;
class ClientNodeHandshake;

class WalletConnectionManager {
public:
	WalletConnectionManager();
	virtual ~WalletConnectionManager();

private:
	HashMap<PubSubId, ClientNodeHandshake> clientHandshakeHash;
};

} /* namespace codablecash */

#endif /* BC_WALLET_NET_WALLETCONNECTIONMANAGER_H_ */
