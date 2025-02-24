/*
 * WalletNetworkManager.cpp
 *
 *  Created on: Feb 17, 2025
 *      Author: iizuka
 */

#include "bc_wallet_net/WalletNetworkManager.h"

namespace codablecash {

WalletNetworkManager::WalletNetworkManager() {
	this->seeder = nullptr;

}

WalletNetworkManager::~WalletNetworkManager() {
	this->seeder = nullptr;
}

void WalletNetworkManager::setSeeder(INetworkSeeder *seeder) noexcept {
	this->seeder = seeder;
}

} /* namespace codablecash */
