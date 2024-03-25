/*
 * HdWallet.cpp
 *
 *  Created on: 2023/01/23
 *      Author: iizuka
 */

#include "bc_wallet/HdWallet.h"
#include "bc_wallet/WalletAccount.h"
#include "bc_wallet/HdWalletSeed.h"

#include "bc_wallet_encoder/IWalletDataEncoder.h"

#include "base_io/File.h"
#include "base_io/ByteBuffer.h"

#include "bc_base_conf_store/StatusStore.h"

#include "base/StackRelease.h"


namespace codablecash {

HdWallet::HdWallet(const File* baseDir) {
	this->baseDir = new File(*baseDir);
	this->accounts = new ArrayList<WalletAccount>();
	this->encodedSeed = nullptr;

	this->store = new StatusStore(baseDir, HdWallet::STORE_NAME);
}

HdWallet::~HdWallet() {
	delete this->store;

	this->accounts->deleteElements();
	delete this->accounts;

	delete this->encodedSeed;

	delete this->baseDir;
}

HdWallet* HdWallet::create(const File *dir, const HdWalletSeed *seed, uint16_t defaultZone, const IWalletDataEncoder* encoder) {
	HdWallet* wallet = new HdWallet(dir);
	wallet->setSeed(seed, encoder);

	wallet->newAccount(seed, defaultZone, encoder);
	wallet->save();

	return wallet;
}

HdWallet* HdWallet::loadWallet(const File *dir, const IWalletDataEncoder *encoder) {
	HdWallet* wallet = new HdWallet(dir);
	wallet->load(encoder);

	return wallet;
}

void HdWallet::setSeed(const HdWalletSeed *seed, const IWalletDataEncoder* encoder) {
	delete this->encodedSeed;

	this->encodedSeed = seed->encodedSeed(encoder);
}

void HdWallet::newAccount(const HdWalletSeed *rootSeed, uint16_t zone, const IWalletDataEncoder* encoder) {
	int accountIndex = this->accounts->size();
	WalletAccount* account = WalletAccount::newAccount(this->baseDir, rootSeed, accountIndex, zone, encoder);

	this->accounts->addElement(account);
	save();
}

void HdWallet::save() {
	{
		ByteBuffer* buff = this->encodedSeed->getByteBuffer();
		this->store->addBinaryValue(KEY_ENCODED_SEED, buff->array(), buff->limit());
	}
	this->store->addShortValue(KEY_NUM_ACCOUNTS, this->accounts->size());
}

void HdWallet::load(const IWalletDataEncoder *encoder) {
	this->store->load();
	{
		ByteBuffer* buff = this->store->getBinaryValue(KEY_ENCODED_SEED); __STP(buff);
		this->encodedSeed = new HdWalletSeed((const char*)buff->array(), buff->limit());
	}

	int numAccounts = this->store->getShortValue(KEY_NUM_ACCOUNTS);
	for(int i = 0; i != numAccounts; ++i){
		WalletAccount* account = WalletAccount::loadAccount(this->baseDir, i, encoder);
		this->accounts->addElement(account);
	}
}

} /* namespace codablecash */
