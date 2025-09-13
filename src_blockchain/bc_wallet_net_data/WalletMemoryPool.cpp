/*
 * WalletMemoryPool.cpp
 *
 *  Created on: Apr 19, 2025
 *      Author: iizuka
 */

#include "bc_wallet_net_data/WalletMemoryPool.h"

#include "base_io/File.h"

#include "random_access_file/DiskCacheManager.h"

#include "btree/Btree.h"
#include "btree/BtreeConfig.h"

#include "base/StackRelease.h"
#include "base/UnicodeString.h"

#include "bc_base_trx_index/TransactionIdKeyFactory.h"
#include "bc_base_trx_index/TransactionDataFactory.h"
#include "bc_base_trx_index/TransactionIdData.h"
#include "bc_base_trx_index/TransactionIdKey.h"
#include "bc_base_trx_index/TransactionData.h"

#include "bc_trx/AbstractBlockchainTransaction.h"


namespace codablecash {

WalletMemoryPool::WalletMemoryPool(File* baseDir) {
	this->baseDir = new File(*baseDir);
	this->cacheManager = new DiskCacheManager();
	this->trxStore = nullptr;
}

WalletMemoryPool::~WalletMemoryPool() {
	close();

	delete this->baseDir;
	delete this->cacheManager;
}

bool WalletMemoryPool::exists() {
	bool exTrxStore = false;
	{
		UnicodeString fileName(NAME_TRX_STORE);

		TransactionIdKeyFactory* keyFactory = new TransactionIdKeyFactory(); __STP(keyFactory);
		TransactionDataFactory* dataFactory = new TransactionDataFactory(); __STP(dataFactory);

		Btree btree(this->baseDir, &fileName, this->cacheManager, keyFactory, dataFactory);
		exTrxStore = btree.exists();
	}

	return exTrxStore;
}

void WalletMemoryPool::createBlankPool() {
	if(this->baseDir->exists()){
		this->baseDir->deleteDir();
	}
	this->baseDir->mkdirs();

	{
		UnicodeString fileName(NAME_TRX_STORE);

		TransactionIdKeyFactory* keyFactory = new TransactionIdKeyFactory(); __STP(keyFactory);
		TransactionDataFactory* dataFactory = new TransactionDataFactory(); __STP(dataFactory);

		Btree btree(this->baseDir, &fileName, this->cacheManager, keyFactory, dataFactory);

		BtreeConfig config;
		config.nodeNumber = 8;
		config.defaultSize = 1024;
		config.blockSize = 32;
		btree.create(&config);
	}

}

void WalletMemoryPool::open() {
	if(!exists()){
		createBlankPool();
	}
	{
		UnicodeString fileName(NAME_TRX_STORE);

		TransactionIdKeyFactory* keyFactory = new TransactionIdKeyFactory(); __STP(keyFactory);
		TransactionDataFactory* dataFactory = new TransactionDataFactory(); __STP(dataFactory);

		this->trxStore = new Btree(this->baseDir, &fileName, this->cacheManager, keyFactory, dataFactory);
		BtreeOpenConfig opconf;
		opconf.numDataBuffer = 256;
		opconf.numNodeBuffer = 512;
		this->trxStore->open(&opconf);
	}
}

void WalletMemoryPool::close() noexcept {
	if(this->trxStore != nullptr){
		this->trxStore->close();
		delete this->trxStore;
		this->trxStore = nullptr;
	}
}

void WalletMemoryPool::putTransaction(const AbstractBlockchainTransaction *trx) {
	const TransactionId* trxId = trx->getTransactionId();
	TransactionIdData data(trxId);

	{
		TransactionIdKey key(trxId);
		TransactionData data(trx);

		this->trxStore->putData(&key, &data);
	}
}

void WalletMemoryPool::removeTransaction(const TransactionId *trxId) {
	AbstractBlockchainTransaction* trx = getBlockchainTransaction(trxId); __STP(trx);

	if(trx != nullptr){
		TransactionIdKey key(trxId);
		this->trxStore->remove(&key);
	}
}

AbstractBlockchainTransaction* WalletMemoryPool::getBlockchainTransaction(const TransactionId *trxId) const {
	TransactionIdKey key(trxId);
	IBlockObject* obj = this->trxStore->findByKey(&key); __STP(obj);
	TransactionData* data = dynamic_cast<TransactionData*>(obj);

	return data != nullptr ? dynamic_cast<AbstractBlockchainTransaction*>(data->getTrx()->copyData()) : nullptr;
}

bool WalletMemoryPool::hasTransaction(const TransactionId *trxId) const {
	AbstractBlockchainTransaction* trx = getBlockchainTransaction(trxId); __STP(trx);

	return trx != nullptr;
}

BtreeScanner* WalletMemoryPool::getScanner() const noexcept {
	return this->trxStore->getScanner();
}

} /* namespace codablecash */
