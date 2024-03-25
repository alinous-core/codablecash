/*
 * TrxRepository.cpp
 *
 *  Created on: 2023/02/23
 *      Author: iizuka
 */

#include "bc_wallet_trx_repo/WalletAccountTrxRepository.h"
#include "bc_wallet_trx_repo/WalletAccountUtxoRepository.h"

#include "bc_base_trx_index/TransactionIdKeyFactory.h"
#include "bc_base_trx_index/TransactionIdKey.h"
#include "bc_base_trx_index/TransactionDataFactory.h"
#include "bc_base_trx_index/TransactionData.h"

#include "bc_wallet/WalletAccount.h"

#include "base/UnicodeString.h"
#include "base/StackRelease.h"

#include "btree/Btree.h"
#include "btree/BtreeConfig.h"

#include "random_access_file/DiskCacheManager.h"

#include "bc_trx/AbstractBlockchainTransaction.h"

#include "bc_base/BalanceUnit.h"

#include "btree/BtreeScanner.h"
namespace codablecash {

const UnicodeString WalletAccountTrxRepository::FILE_NAME(L"transactions");

WalletAccountTrxRepository::WalletAccountTrxRepository(WalletAccount* account) {
	this->account = account;
	this->utxoRepo = nullptr;
	this->cacheManager = new DiskCacheManager();
	this->btree = nullptr;
}

WalletAccountTrxRepository::~WalletAccountTrxRepository() {
	close();

	this->account = nullptr;
	delete this->cacheManager;
}

void WalletAccountTrxRepository::init() {
	initSelf();

	this->utxoRepo = new WalletAccountUtxoRepository(this->account->getAccountBaseDir());

	// create btree
	this->utxoRepo->init();

	close();
}

void WalletAccountTrxRepository::open() {
	openSelf();

	this->utxoRepo = new WalletAccountUtxoRepository(this->account->getAccountBaseDir());
	this->utxoRepo->open();
}

void WalletAccountTrxRepository::close() {
	closeSelf();

	if(this->utxoRepo != nullptr){
		this->utxoRepo->close();
		delete this->utxoRepo;
		this->utxoRepo = nullptr;
	}
}

void WalletAccountTrxRepository::initSelf() {
	UnicodeString fileName(WalletAccountTrxRepository::FILE_NAME);

	TransactionIdKeyFactory* keyFactory = new TransactionIdKeyFactory(); __STP(keyFactory);
	TransactionDataFactory* dataFactory = new TransactionDataFactory(); __STP(dataFactory);

	Btree btree(this->account->getAccountBaseDir(), &fileName, this->cacheManager, keyFactory, dataFactory);

	BtreeConfig config;
	config.nodeNumber = 8;
	config.defaultSize = 1024;
	config.blockSize = 32;
	btree.create(&config);
}

void WalletAccountTrxRepository::openSelf() {
	closeSelf();

	UnicodeString fileName(WalletAccountTrxRepository::FILE_NAME);

	TransactionIdKeyFactory* keyFactory = new TransactionIdKeyFactory(); __STP(keyFactory);
	TransactionDataFactory* dataFactory = new TransactionDataFactory(); __STP(dataFactory);

	this->btree = new Btree(this->account->getAccountBaseDir(), &fileName, this->cacheManager, keyFactory, dataFactory);

	BtreeOpenConfig opconf;
	opconf.numDataBuffer = 256;
	opconf.numNodeBuffer = 512;
	this->btree->open(&opconf);
}

BalanceUtxo* WalletAccountTrxRepository::getBalanceUtxo(const UtxoId *utxoId) {
	return this->utxoRepo->getBalanceUtxo(utxoId);
}

void WalletAccountTrxRepository::closeSelf() {
	if(this->btree != nullptr){
		this->btree->close();
		delete this->btree;
		this->btree = nullptr;
	}
}

void WalletAccountTrxRepository::importUtxo(const BalanceUtxo *utxo) {
	this->utxoRepo->importUtxo(utxo);
}

void WalletAccountTrxRepository::importTransaction(const AbstractBlockchainTransaction *trx) {
	TransactionIdKey key(trx->getTransactionId());
	TransactionData data(trx);

	this->btree->putData(&key, &data);
}

AbstractBlockchainTransaction* WalletAccountTrxRepository::findTransaction(const TransactionId *trxId) {
	TransactionIdKey key(trxId);
	IBlockObject* object = this->btree->findByKey(&key); __STP(object);
	TransactionData* data = dynamic_cast<TransactionData*>(object);

	return data != nullptr ? dynamic_cast<AbstractBlockchainTransaction*>(data->getTrx()->copyData()) : nullptr;;
}

void WalletAccountTrxRepository::removeTransaction(const TransactionId *trxId) {
	TransactionIdKey key(trxId);
	this->btree->remove(&key);
}

void WalletAccountTrxRepository::removeUtxo(const UtxoId *utxoId) {
	this->utxoRepo->removeUtxo(utxoId);
}

BalanceUnit WalletAccountTrxRepository::getTotalAmount() const noexcept {
	return this->utxoRepo->getTotalAmount();
}

ArrayList<BalanceUtxo>* WalletAccountTrxRepository::getBalanceUtxos(const AddressDescriptor *desc) {
	return this->utxoRepo->getBalanceUtxos(desc);
}

BtreeScanner* WalletAccountTrxRepository::getScanner() const {
	BtreeScanner* scanner = this->btree->getScanner();
	scanner->begin();

	return scanner;
}

} /* namespace codablecash */
