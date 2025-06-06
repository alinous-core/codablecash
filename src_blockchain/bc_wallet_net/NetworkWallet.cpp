/*
 * NetworkWallet.cpp
 *
 *  Created on: Feb 17, 2025
 *      Author: iizuka
 */

#include "bc_wallet_net/NetworkWallet.h"
#include "bc_wallet_net/WalletNetworkManager.h"
#include "bc_wallet_net/StakingSeedManager.h"
#include "bc_wallet_net/NetworkWalletData.h"

#include "bc_wallet_net_processor/NetworkClientCommandProcessor.h"

#include "bc_wallet_filter/BloomFilter512.h"

#include "bc_wallet/HdWallet.h"
#include "bc_wallet/HdWalletSeed.h"
#include "bc_wallet/WalletAccount.h"

#include "base/StackRelease.h"
#include "base/UnicodeString.h"

#include "base_io/File.h"

#include "bc_wallet_encoder/PasswordEncoder.h"

#include "bc/CodablecashSystemParam.h"


namespace codablecash {

NetworkWallet::NetworkWallet(const File* baseDir, ISystemLogger* logger, const CodablecashSystemParam *config) {
	this->baseDir = new File(*baseDir);
	this->logger = logger;
	this->config = new CodablecashSystemParam(*config);
	this->networkManager = nullptr;

	this->stakingSeedManager = new StakingSeedManager(baseDir);

	this->clientCommandProcessor = new NetworkClientCommandProcessor(this, logger);

	File* dataBaseDir = baseDir->get(L"data"); __STP(dataBaseDir);
	this->walletData = new NetworkWalletData(dataBaseDir, logger);
}

NetworkWallet::~NetworkWallet() {
	delete this->walletData;
	delete this->baseDir;

	delete this->config;
	delete this->networkManager;
	delete this->stakingSeedManager;
	delete this->clientCommandProcessor;

	this->logger = nullptr;
}

void NetworkWallet::init() {
	this->clientCommandProcessor->init();
}

NetworkWallet* NetworkWallet::createNewWallet(const File *dir, const UnicodeString *pass, uint16_t zone, int defaultMaxAddress, ISystemLogger* logger, const CodablecashSystemParam *config) {
	NetworkWallet* wallet = new NetworkWallet(dir, logger, config); __STP(wallet);
	wallet->init();

	HdWalletSeed* seed = HdWalletSeed::newSeed(); __STP(seed);
	PasswordEncoder enc(pass);

	wallet->doCreateWallet(&enc, seed, zone, defaultMaxAddress);
	wallet->createData();

	return __STP_MV(wallet);
}



void NetworkWallet::doCreateWallet(const IWalletDataEncoder *encoder, const HdWalletSeed *seed, uint16_t zone, int defaultMaxAddress) {
	this->walletData->createHdWallet(seed, zone, encoder, defaultMaxAddress);
}

HdWalletSeed* NetworkWallet::getRootSeed(const IWalletDataEncoder* encoder) const {
	HdWallet* hdWallet = this->walletData->getHdWallet();

	return hdWallet->getRootSeed(encoder);
}

NetworkWallet* NetworkWallet::resotreWallet(const File *dir, const UnicodeString *pass, uint16_t zone, const HdWalletSeed* rootSeed, int defaultMaxAddress, ISystemLogger* logger, const CodablecashSystemParam *config) {
	NetworkWallet* wallet = new NetworkWallet(dir, logger, config); __STP(wallet);
	wallet->init();

	PasswordEncoder enc(pass);
	wallet->doCreateWallet(&enc, rootSeed, zone, defaultMaxAddress);
	wallet->createData();

	return __STP_MV(wallet);
}

void NetworkWallet::setNetworkSeeder(INetworkSeeder *seeder) noexcept {
	HdWallet* hdWallet = this->walletData->getHdWallet();

	if(hdWallet != nullptr && this->networkManager == nullptr){
		uint16_t zone = hdWallet->getDefaultZone();
		this->networkManager = new WalletNetworkManager(zone, this->logger, this->clientCommandProcessor);
	}

	this->networkManager->setSeeder(seeder);
}

AddressDescriptor* NetworkWallet::getAddressDescriptor(int accountIndex, int addressIndex) const {
	HdWallet* hdWallet = this->walletData->getHdWallet();

	WalletAccount* account = hdWallet->getAccount(accountIndex);

	return account->getReceivingAddressDescriptor(addressIndex);
}

void NetworkWallet::initNetwork(INetworkSeeder *seeder, const IWalletDataEncoder* encoder) {
	setNetworkSeeder(seeder);

	// start client processor
	{
		this->clientCommandProcessor->startProcessors(true);
	}

	HdWallet* hdWallet = this->walletData->getHdWallet();
	ArrayList<BloomFilter512>* filters = hdWallet->getBloomFilters(encoder); __STP(filters);
	filters->setDeleteOnExit();

	this->networkManager->maintainNetwork(filters);
}

void NetworkWallet::createData() {
	this->walletData->createStores();

	this->clientCommandProcessor->createBlank();
}

void NetworkWallet::startNetwork() {
	// sync;
	syncBlockchain();

	this->clientCommandProcessor->resumeRequestProcessor();
}

void NetworkWallet::syncBlockchain() {
	// FIXME syncBlockchain

}
} /* namespace codablecash */
