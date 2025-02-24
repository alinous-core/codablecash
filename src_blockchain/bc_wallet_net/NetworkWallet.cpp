/*
 * NetworkWallet.cpp
 *
 *  Created on: Feb 17, 2025
 *      Author: iizuka
 */

#include "bc_wallet_net/NetworkWallet.h"
#include "bc_wallet_net/WalletNetworkManager.h"

#include "bc_wallet/HdWallet.h"
#include "bc_wallet/HdWalletSeed.h"

#include "base/StackRelease.h"
#include "base/UnicodeString.h"

#include "base_io/File.h"

#include "bc_wallet_encoder/PasswordEncoder.h"

namespace codablecash {

NetworkWallet::NetworkWallet(const File* baseDir) {
	this->baseDir = new File(*baseDir);
	this->hdWallet = nullptr;
	this->networkManager = nullptr;
	this->encoder = nullptr;
}

NetworkWallet::~NetworkWallet() {
	delete this->hdWallet;
	delete this->baseDir;

	delete this->networkManager;
	delete this->encoder;
}

NetworkWallet* NetworkWallet::createNewWallet(const File *dir, const UnicodeString *pass, uint16_t zone) {
	NetworkWallet* wallet = new NetworkWallet(dir); __STP(wallet);

	HdWalletSeed* seed = HdWalletSeed::newSeed(); __STP(seed);
	PasswordEncoder enc(pass);

	wallet->doCreateWallet(&enc, seed, zone);

	return __STP_MV(wallet);
}

void NetworkWallet::doCreateWallet(const IWalletDataEncoder *encoder, const HdWalletSeed *seed, uint16_t zone) {
	delete this->encoder;
	this->encoder = encoder->copy();

	File* hdbase = this->baseDir->get(L"hd"); __STP(hdbase);

	this->hdWallet = HdWallet::create(hdbase, seed, zone, this->encoder);

}

HdWalletSeed* NetworkWallet::getRootSeed() const {
	return this->hdWallet->getRootSeed(this->encoder);
}

NetworkWallet* NetworkWallet::resotreWallet(const File *dir, const UnicodeString *pass, uint16_t zone, const HdWalletSeed* rootSeed) {
	NetworkWallet* wallet = new NetworkWallet(dir); __STP(wallet);

	PasswordEncoder enc(pass);
	wallet->doCreateWallet(&enc, rootSeed, zone);

	return __STP_MV(wallet);
}

void NetworkWallet::setNetworkSeeder(INetworkSeeder *seeder) noexcept {
	if(this->networkManager == nullptr){
		this->networkManager = new WalletNetworkManager();
	}

	this->networkManager->setSeeder(seeder);
}

} /* namespace codablecash */
