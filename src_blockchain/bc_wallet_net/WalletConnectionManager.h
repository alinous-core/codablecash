/*
 * WalletConnectionManager.h
 *
 *  Created on: Feb 21, 2025
 *      Author: iizuka
 */

#ifndef BC_WALLET_NET_WALLETCONNECTIONMANAGER_H_
#define BC_WALLET_NET_WALLETCONNECTIONMANAGER_H_

#include "base/HashMap.h"

namespace alinous {
class SysMutex;
}

using namespace alinous;

namespace codablecash {

class PubSubId;
class ClientNodeHandshake;
class P2pNodeRecord;
class ISystemLogger;
class NodeIdentifierSource;
class NetworkClientCommandProcessor;

class WalletConnectionManager {
public:
	WalletConnectionManager(uint16_t defaultZone, NetworkClientCommandProcessor* clientCommandProcessor);
	virtual ~WalletConnectionManager();

	void disconnect(const PubSubId* pubsubId);
	void clearDeleteList();
	bool isClearedAll() const noexcept;
	void dispose();

	int getNumConnection() const noexcept;
	bool connect(int protocol, const UnicodeString* host, uint32_t port, ISystemLogger* logger);

private:
	void __disconnect(const PubSubId* pubsubId);

private:
	HashMap<PubSubId, ClientNodeHandshake> clientHandshakeHash;
	ArrayList<ClientNodeHandshake> list;
	SysMutex* mutex;

	uint16_t defaultZone;
	NodeIdentifierSource* source;

	// command processor
	NetworkClientCommandProcessor* clientCommandProcessor;
};

} /* namespace codablecash */

#endif /* BC_WALLET_NET_WALLETCONNECTIONMANAGER_H_ */
