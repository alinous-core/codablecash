/*
 * NetworkWallet.cpp
 *
 *  Created on: Feb 17, 2025
 *      Author: iizuka
 */

#include "bc_wallet_net/NetworkWallet.h"
#include "bc_wallet_net/WalletNetworkManager.h"
#include "bc_wallet_net/StakingSeedManager.h"
#include "bc_wallet_net/NetworkClientCommandProcessor.h"

#include "bc_wallet/HdWallet.h"
#include "bc_wallet/HdWalletSeed.h"
#include "bc_wallet/WalletAccount.h"

#include "base/StackRelease.h"
#include "base/UnicodeString.h"

#include "base_io/File.h"

#include "bc_wallet_encoder/PasswordEncoder.h"


namespace codablecash {

NetworkWallet::NetworkWallet(const File* baseDir, ISystemLogger* logger) {
	this->baseDir = new File(*baseDir);
	this->logger = logger;
	this->hdWallet = nullptr;
	this->networkManager = nullptr;

	this->stakingSeedManager = new StakingSeedManager(baseDir);

	this->clientCommandProcessor = new NetworkClientCommandProcessor(this);
}

NetworkWallet::~NetworkWallet() {
	delete this->hdWallet;
	delete this->baseDir;

	delete this->networkManager;
	delete this->stakingSeedManager;
	delete this->clientCommandProcessor;

	this->logger = nullptr;
}

NetworkWallet* NetworkWallet::createNewWallet(const File *dir, const UnicodeString *pass, uint16_t zone, int defaultMaxAddress, ISystemLogger* logger) {
	NetworkWallet* wallet = new NetworkWallet(dir, logger); __STP(wallet);

	HdWalletSeed* seed = HdWalletSeed::newSeed(); __STP(seed);
	PasswordEncoder enc(pass);

	wallet->doCreateWallet(&enc, seed, zone, defaultMaxAddress);

	return __STP_MV(wallet);
}



void NetworkWallet::doCreateWallet(const IWalletDataEncoder *encoder, const HdWalletSeed *seed, uint16_t zone, int defaultMaxAddress) {
	File* hdbase = this->baseDir->get(L"hd"); __STP(hdbase);

	this->hdWallet = HdWallet::create(hdbase, seed, zone, encoder, defaultMaxAddress);

}

HdWalletSeed* NetworkWallet::getRootSeed(const IWalletDataEncoder* encoder) const {
	return this->hdWallet->getRootSeed(encoder);
}

NetworkWallet* NetworkWallet::resotreWallet(const File *dir, const UnicodeString *pass, uint16_t zone, const HdWalletSeed* rootSeed, int defaultMaxAddress, ISystemLogger* logger) {
	NetworkWallet* wallet = new NetworkWallet(dir, logger); __STP(wallet);

	PasswordEncoder enc(pass);
	wallet->doCreateWallet(&enc, rootSeed, zone, defaultMaxAddress);

	return __STP_MV(wallet);
}

void NetworkWallet::setNetworkSeeder(INetworkSeeder *seeder) noexcept {
	if(this->networkManager == nullptr){
		uint16_t zone = this->hdWallet->getDefaultZone();
		this->networkManager = new WalletNetworkManager(zone, this->logger, this->clientCommandProcessor);
	}

	this->networkManager->setSeeder(seeder);
}

AddressDescriptor* NetworkWallet::getAddressDescriptor(int accountIndex, int addressIndex) const {
	WalletAccount* account = this->hdWallet->getAccount(accountIndex);

	return account->getReceivingAddressDescriptor(addressIndex);
}

void NetworkWallet::initNetwork() {
	this->networkManager->maintainNetwork();

}

} /* namespace codablecash */
