/*
 * WalletNetworkManager.h
 *
 *  Created on: Feb 17, 2025
 *      Author: iizuka
 */

#ifndef BC_WALLET_NET_WALLETNETWORKMANAGER_H_
#define BC_WALLET_NET_WALLETNETWORKMANAGER_H_

namespace codablecash {

class INetworkSeeder;

class WalletNetworkManager {
public:
	WalletNetworkManager();
	virtual ~WalletNetworkManager();

	void setSeeder(INetworkSeeder* seeder) noexcept;

private:
	INetworkSeeder* seeder;
};

} /* namespace codablecash */

#endif /* BC_WALLET_NET_WALLETNETWORKMANAGER_H_ */
