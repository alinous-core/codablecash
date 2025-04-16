/*
 * NetworkCommandData.cpp
 *
 *  Created on: Mar 23, 2025
 *      Author: iizuka
 */

#include "bc_wallet_net/NetworkWalletData.h"

#include "bc_wallet/HdWallet.h"

#include "base/StackRelease.h"

#include "base_io/File.h"

#include "bc_blockstore_header/BlockHeaderStoreManager.h"

#include "bc_blockstore/CodablecashBlockchain.h"

#include "bc_status_cache/HeadBlockDetector.h"

namespace codablecash {

NetworkWalletData::NetworkWalletData(const File* baseDir, ISystemLogger* logger) {
	this->baseDir = new File(*baseDir);
	this->logger = logger;
	this->hdWallet = nullptr;
	this->headerManager = nullptr;
	this->detector = nullptr;
}

NetworkWalletData::~NetworkWalletData() {
	delete this->hdWallet;

	delete this->headerManager;
	delete this->detector;

	delete this->baseDir;
	this->logger = nullptr;
}

void NetworkWalletData::createHdWallet(const HdWalletSeed *seed, uint16_t defaultZone, const IWalletDataEncoder *encoder, int defaultMaxAddress) {
	delete this->hdWallet;

	File* hdbase = this->baseDir->get(L"hd"); __STP(hdbase);

	this->hdWallet = HdWallet::create(hdbase, seed, defaultZone, encoder, defaultMaxAddress);
}

void NetworkWalletData::createStores() {
	this->headerManager = new BlockHeaderStoreManager(this->baseDir, CodablecashBlockchain::DEFAULT_SECTION_LIMIT);
	this->detector = new HeadBlockDetector(this->logger);

}

} /* namespace codablecash */
