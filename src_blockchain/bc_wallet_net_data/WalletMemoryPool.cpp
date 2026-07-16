/*
 * WalletMemoryPool.cpp
 *
 *  Created on: Apr 19, 2025
 *      Author: iizuka
 */

#include "bc_wallet_net_data/WalletMemoryPool.h"
#include "bc_wallet_net_data/WalletTransactionIdListDataFactory.h"
#include "bc_wallet_net_data/WalletTransactionIdListData.h"
#include "bc_wallet_net_data/WalletMemoryPoolScanner.h"

#include "base_io/File.h"

#include "random_access_file/DiskCacheManager.h"

#include "btree/Btree.h"
#include "btree/BtreeConfig.h"

#include "base/StackRelease.h"
#include "base/UnicodeString.h"

#include "bc_base_trx_index/TransactionIdKeyFactory.h"
#include "bc_base_trx_index/TransactionDataFactory.h"
#include "bc_base_trx_index/TransactionIdKey.h"
#include "bc_base_trx_index/TransactionData.h"

#include "bc_trx/AbstractBlockchainTransaction.h"
#include "bc_trx/TransactionId.h"

#include "base_timestamp/SystemTimestampKeyFactory.h"
#include "base_timestamp/SystemTimestampKey.h"


namespace codablecash {

WalletMemoryPool::WalletMemoryPool(File* baseDir) {
	this->baseDir = new File(*baseDir);
	this->cacheManager = new DiskCacheManager();
	this->trxStore = nullptr;
	this->trxOrderIndex = nullptr;
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
	{
		UnicodeString fileName(NAME_TRX_ORDER_INDEX);

		SystemTimestampKeyFactory* keyFactory = new SystemTimestampKeyFactory(); __STP(keyFactory);
		WalletTransactionIdListDataFactory* dataFactory = new WalletTransactionIdListDataFactory(); __STP(dataFactory);

		Btree btree(this->baseDir, &fileName, this->cacheManager, keyFactory, dataFactory);
		exTrxStore |= btree.exists();
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

	{
		UnicodeString fileName(NAME_TRX_ORDER_INDEX);

		SystemTimestampKeyFactory* keyFactory = new SystemTimestampKeyFactory(); __STP(keyFactory);
		WalletTransactionIdListDataFactory* dataFactory = new WalletTransactionIdListDataFactory(); __STP(dataFactory);

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

	{
		UnicodeString fileName(NAME_TRX_ORDER_INDEX);

		SystemTimestampKeyFactory* keyFactory = new SystemTimestampKeyFactory(); __STP(keyFactory);
		WalletTransactionIdListDataFactory* dataFactory = new WalletTransactionIdListDataFactory(); __STP(dataFactory);

		this->trxOrderIndex = new Btree(this->baseDir, &fileName, this->cacheManager, keyFactory, dataFactory);
		BtreeOpenConfig opconf;
		opconf.numDataBuffer = 256;
		opconf.numNodeBuffer = 512;
		this->trxOrderIndex->open(&opconf);
	}
}

void WalletMemoryPool::close() noexcept {
	if(this->trxStore != nullptr){
		this->trxStore->close();
		delete this->trxStore;
		this->trxStore = nullptr;
	}
	if(this->trxOrderIndex != nullptr){
		this->trxOrderIndex->close();
		delete this->trxOrderIndex;
		this->trxOrderIndex = nullptr;
	}
}

void WalletMemoryPool::putTransaction(const AbstractBlockchainTransaction *trx) {
	const TransactionId* trxId = trx->getTransactionId();

	{
		TransactionIdKey key(trxId);
		TransactionData data(trx);

		this->trxStore->putData(&key, &data);
	}

	{
		const SystemTimestamp* tm = trx->getTimestamp();
		SystemTimestampKey key(tm);
		WalletTransactionIdListData data;
		data.add(trxId);

		this->trxOrderIndex->putData(&key, &data);
	}
}

void WalletMemoryPool::removeTransaction(const TransactionId *trxId) {
	AbstractBlockchainTransaction* trx = getBlockchainTransaction(trxId); __STP(trx);

	if(trx != nullptr){
		{
			TransactionIdKey key(trxId);
			this->trxStore->remove(&key);
		}

		{
			const SystemTimestamp* tm = trx->getTimestamp();
			SystemTimestampKey key(tm);
			key.setRemoveKey(trxId);
			this->trxOrderIndex->remove(&key);
		}
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

WalletMemoryPoolScanner* WalletMemoryPool::getScanner() const noexcept {
	WalletMemoryPoolScanner* scanner = new WalletMemoryPoolScanner(this);
	return scanner;
}

BtreeScanner* WalletMemoryPool::getBtreeScanner() const noexcept {
	BtreeScanner* scanner = this->trxOrderIndex->getScanner();
	return scanner;
}

} /* namespace codablecash */
