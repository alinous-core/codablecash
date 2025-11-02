/*
 * WalletConfig.cpp
 *
 *  Created on: Apr 18, 2025
 *      Author: iizuka
 */

#include "bc_wallet_net/WalletConfig.h"

namespace codablecash {

WalletConfig::WalletConfig(const WalletConfig &inst) {
	this->numInitialSyncNodes = inst.numInitialSyncNodes;
}

WalletConfig::WalletConfig() {
	this->numInitialSyncNodes = 3;
}

WalletConfig::~WalletConfig() {

}

} /* namespace codablecash */
