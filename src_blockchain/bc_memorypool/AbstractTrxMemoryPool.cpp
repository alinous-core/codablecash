/*
 * AbstractTrxMemoryPool.cpp
 *
 *  Created on: 2023/12/02
 *      Author: iizuka
 */

#include "bc_memorypool/AbstractTrxMemoryPool.h"
#include "bc_memorypool/MempoolUtxoFindResult.h"

#include "random_access_file/DiskCacheManager.h"

#include "bc_base_trx_index/TransactionIdKeyFactory.h"
#include "bc_base_trx_index/BalanceUnitKeyFactory.h"
#include "bc_base_trx_index/TransactionIdsListDataFactory.h"
#include "bc_base_trx_index/TransactionIdDataFactory.h"
#include "bc_base_trx_index/TransactionDataFactory.h"

#include "bc_base_utxo_index/UtxoIdKeyFactory.h"

#include "base/StackRelease.h"
#include "base/UnicodeString.h"

#include "base_io/File.h"

#include "btree/Btree.h"
#include "btree/BtreeConfig.h"
#include "btree/BtreeReverseScanner.h"

#include "bc_base_trx_index/TransactionIdData.h"
#include "bc_base_trx_index/TransactionIdKey.h"
#include "bc_base_trx_index/TransactionData.h"
#include "bc_base_trx_index/BalanceUnitKey.h"
#include "bc_base_trx_index/TransactionIdsListData.h"

#include "bc_base_utxo_index/UtxoIdKey.h"

#include "bc_trx/UtxoId.h"
#include "bc_trx/AbstractBlockchainTransaction.h"
#include "bc_trx/AbstractUtxo.h"
#include "bc_trx/TransactionId.h"

#include "bc_base/BalanceUnit.h"

namespace codablecash {

AbstractTrxMemoryPool::AbstractTrxMemoryPool(const File* baseDir) {
	this->baseDir = new File(*baseDir);

	this->trxStore = nullptr;
	this->feeRateIndex = nullptr;
	this->utxoIndex = nullptr;

	this->cacheManager = new DiskCacheManager();

}

AbstractTrxMemoryPool::~AbstractTrxMemoryPool() {
	close();
	delete this->baseDir;
	delete this->cacheManager;
}

bool AbstractTrxMemoryPool::exists() {
	bool exTrxStore = false;
	{
		UnicodeString fileName(NAME_TRX_STORE);

		TransactionIdKeyFactory* keyFactory = new TransactionIdKeyFactory(); __STP(keyFactory);
		TransactionDataFactory* dataFactory = new TransactionDataFactory(); __STP(dataFactory);

		Btree btree(this->baseDir, &fileName, this->cacheManager, keyFactory, dataFactory);
		exTrxStore = btree.exists();
	}

	bool exfeeRateIndex = false;
	{
		UnicodeString fileName(NAME_FEE_RATE_INDEX);

		BalanceUnitKeyFactory* keyFactory = new BalanceUnitKeyFactory(); __STP(keyFactory);
		TransactionIdsListDataFactory* dataFactory = new TransactionIdsListDataFactory(); __STP(dataFactory);

		Btree btree(this->baseDir, &fileName, this->cacheManager, keyFactory, dataFactory);
		exfeeRateIndex = btree.exists();
	}

	bool exUtxoIndex = false;
	{
		UnicodeString fileName(NAME_UTXO_INDEX);

		UtxoIdKeyFactory* keyFactory = new UtxoIdKeyFactory(); __STP(keyFactory);
		TransactionIdDataFactory* dataFactory = new TransactionIdDataFactory(); __STP(dataFactory);

		Btree btree(this->baseDir, &fileName, this->cacheManager, keyFactory, dataFactory);
		exUtxoIndex = btree.exists();
	}

	return exTrxStore && exfeeRateIndex && exUtxoIndex;
}

void AbstractTrxMemoryPool::createBlankPool() {
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
		UnicodeString fileName(NAME_FEE_RATE_INDEX);

		BalanceUnitKeyFactory* keyFactory = new BalanceUnitKeyFactory(); __STP(keyFactory);
		TransactionIdsListDataFactory* dataFactory = new TransactionIdsListDataFactory(); __STP(dataFactory);

		Btree btree(this->baseDir, &fileName, this->cacheManager, keyFactory, dataFactory);

		BtreeConfig config;
		config.nodeNumber = 8;
		config.defaultSize = 1024;
		config.blockSize = 32;
		btree.create(&config);
	}

	{
		UnicodeString fileName(NAME_UTXO_INDEX);

		UtxoIdKeyFactory* keyFactory = new UtxoIdKeyFactory(); __STP(keyFactory);
		TransactionIdDataFactory* dataFactory = new TransactionIdDataFactory(); __STP(dataFactory);

		Btree btree(this->baseDir, &fileName, this->cacheManager, keyFactory, dataFactory);

		BtreeConfig config;
		config.nodeNumber = 8;
		config.defaultSize = 1024;
		config.blockSize = 32;
		btree.create(&config);
	}
}

void AbstractTrxMemoryPool::open() {
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
		UnicodeString fileName(NAME_FEE_RATE_INDEX);

		BalanceUnitKeyFactory* keyFactory = new BalanceUnitKeyFactory(); __STP(keyFactory);
		TransactionIdsListDataFactory* dataFactory = new TransactionIdsListDataFactory(); __STP(dataFactory);

		this->feeRateIndex = new Btree(this->baseDir, &fileName, this->cacheManager, keyFactory, dataFactory);
		BtreeOpenConfig opconf;
		opconf.numDataBuffer = 256;
		opconf.numNodeBuffer = 512;
		this->feeRateIndex->open(&opconf);
	}

	{
		UnicodeString fileName(NAME_UTXO_INDEX);

		UtxoIdKeyFactory* keyFactory = new UtxoIdKeyFactory(); __STP(keyFactory);
		TransactionIdDataFactory* dataFactory = new TransactionIdDataFactory(); __STP(dataFactory);

		this->utxoIndex = new Btree(this->baseDir, &fileName, this->cacheManager, keyFactory, dataFactory);
		BtreeOpenConfig opconf;
		opconf.numDataBuffer = 256;
		opconf.numNodeBuffer = 512;
		this->utxoIndex->open(&opconf);
	}
}

void AbstractTrxMemoryPool::close() noexcept {
	__close(&this->trxStore);
	__close(&this->feeRateIndex);
	__close(&this->utxoIndex);
}

void AbstractTrxMemoryPool::__close(Btree **tree) noexcept {
	if(*tree != nullptr){
		(*tree)->close();
		delete (*tree);
		(*tree) = nullptr;
	}
}

void AbstractTrxMemoryPool::putTransaction(const AbstractBlockchainTransaction *trx) {
	const TransactionId* trxId = trx->getTransactionId();
	TransactionIdData data(trxId);

	{
		TransactionIdKey key(trxId);
		TransactionData data(trx);

		this->trxStore->putData(&key, &data);
	}

	{
		BalanceUnit feeRate = trx->getFeeRate();
		BalanceUnitKey key(&feeRate);
		TransactionIdsListData listData;
		listData.add(trxId);

		this->feeRateIndex->putData(&key, &listData);
	}

	{
		int maxLoop = trx->getUtxoSize();
		for(int i = 0; i != maxLoop; ++i){
			const AbstractUtxo* utxo = trx->getUtxo(i);
			const UtxoId* utxoId = utxo->getId();

			UtxoIdKey key(utxoId);
			this->utxoIndex->putData(&key, &data);
		}
	}
}

void AbstractTrxMemoryPool::removeTransaction(const TransactionId* trxId) {
	AbstractBlockchainTransaction* trx = getBlockchainTransaction(trxId); __STP(trx);

	if(trx != nullptr){
		TransactionIdKey key(trxId);
		{
			int maxLoop = trx->getUtxoSize();
			for(int i = 0; i != maxLoop; ++i){
				const AbstractUtxo* utxo = trx->getUtxo(i);
				const UtxoId* utxoId = utxo->getId();

				UtxoIdKey key(utxoId);
				this->utxoIndex->remove(&key);
			}
		}

		{
			BalanceUnit feeRate = trx->getFeeRate();
			BalanceUnitKey key(&feeRate);
			key.setTransactionId(trxId);

			this->feeRateIndex->remove(&key);
		}

		{
			this->trxStore->remove(&key);
		}
	}
}

AbstractBlockchainTransaction* AbstractTrxMemoryPool::getBlockchainTransaction(const TransactionId *trxId) const {
	TransactionIdKey key(trxId);
	IBlockObject* obj = this->trxStore->findByKey(&key); __STP(obj);
	TransactionData* data = dynamic_cast<TransactionData*>(obj);

	return data != nullptr ? dynamic_cast<AbstractBlockchainTransaction*>(data->getTrx()->copyData()) : nullptr;
}

TransactionId* AbstractTrxMemoryPool::getTransactionIdFromUtxoId(const UtxoId *utxoId) const {
	UtxoIdKey key(utxoId);
	IBlockObject* obj = this->utxoIndex->findByKey(&key); __STP(obj);
	TransactionIdData* data = dynamic_cast<TransactionIdData*>(obj);

	return data != nullptr ? dynamic_cast<TransactionId*>(data->getTrxId()->copyData()) : nullptr;
}

MempoolUtxoFindResult* AbstractTrxMemoryPool::getUtxo(const UtxoId *utxoId) const {
	TransactionId* trxId = getTransactionIdFromUtxoId(utxoId); __STP(trxId);

	MempoolUtxoFindResult* result = nullptr;
	if(trxId != nullptr){
		AbstractBlockchainTransaction* trx = getBlockchainTransaction(trxId); __STP(trx);

		int maxLoop = trx->getUtxoSize();
		for(int i = 0; i != maxLoop; ++i){
			AbstractUtxo* u = trx->getUtxo(i);

			if(utxoId->equals(u->getId())){
				result = new MempoolUtxoFindResult(trxId, u);
				break;
			}
		}
	}

	return result;
}

BtreeReverseScanner* AbstractTrxMemoryPool::getScanner() const noexcept {
	return this->feeRateIndex->getReverseScanner();
}

BtreeScanner* AbstractTrxMemoryPool::getSyncScanner() const noexcept {
	return this->trxStore->getScanner();
}

int AbstractTrxMemoryPool::getCount() const {
	int count = 0;

	BtreeReverseScanner* scanner = getScanner(); __STP(scanner);
	scanner->begin();

	while(scanner->hasPrevious()){
		const IBlockObject* obj = scanner->previous();
		const TransactionIdsListData* data = dynamic_cast<const TransactionIdsListData*>(obj);
		count += data->getList()->size();
	}

	return count;
}

} /* namespace codablecash */
