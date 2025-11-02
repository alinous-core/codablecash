/*
 * WalletConfig.h
 *
 *  Created on: Apr 18, 2025
 *      Author: iizuka
 */

#ifndef BC_WALLET_NET_WALLETCONFIG_H_
#define BC_WALLET_NET_WALLETCONFIG_H_

namespace codablecash {

class WalletConfig {
public:
	WalletConfig(const WalletConfig& inst);
	WalletConfig();
	virtual ~WalletConfig();

	int getNumInitialSyncNodes() const noexcept {
		return this->numInitialSyncNodes;
	}

private:
	int numInitialSyncNodes;
};

} /* namespace codablecash */

#endif /* BC_WALLET_NET_WALLETCONFIG_H_ */
