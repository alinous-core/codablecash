/*
 * RetriableAccessContainer.h
 *
 *  Created on: Aug 10, 2025
 *      Author: iizuka
 */

#ifndef BC_WALLET_NET_ACCESS_RETRIABLECLIENTACCESSCONTAINER_H_
#define BC_WALLET_NET_ACCESS_RETRIABLECLIENTACCESSCONTAINER_H_

namespace codablecash {

class IRetriableClientAccess;
class ISystemLogger;
class WalletNetworkManager;

class RetriableClientAccessContainer {
public:
	RetriableClientAccessContainer(IRetriableClientAccess* access, ISystemLogger* logger, WalletNetworkManager* networkManager);
	virtual ~RetriableClientAccessContainer();

	void clientAccess();

private:
	IRetriableClientAccess* access;
	ISystemLogger* logger;
	WalletNetworkManager* networkManager;

};

} /* namespace codablecash */

#endif /* BC_WALLET_NET_ACCESS_RETRIABLECLIENTACCESSCONTAINER_H_ */
