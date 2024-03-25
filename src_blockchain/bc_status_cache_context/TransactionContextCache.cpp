/*
 * TransactionContextCache.cpp
 *
 *  Created on: 2023/04/24
 *      Author: iizuka
 */

#include "bc_status_cache_context/TransactionContextCache.h"

#include "base/StackRelease.h"
#include "base/UnicodeString.h"

#include "base_io/File.h"

#include "random_access_file/DiskCacheManager.h"

#include "bc_base_trx_index/TransactionIdKeyFactory.h"
#include "bc_base_trx_index/TransactionDataFactory.h"
#include "bc_base_trx_index/TransactionIdData.h"
#include "bc_base_trx_index/TransactionIdKey.h"
#include "bc_base_trx_index/TransactionData.h"

#include "btree/Btree.h"
#include "btree/BtreeConfig.h"

#include "bc_trx/TransactionId.h"
#include "bc_trx/AbstractBlockchainTransaction.h"


namespace codablecash {

TransactionContextCache::TransactionContextCache(const File* baseDir) {
	this->baseDir = new File(*baseDir);
	this->cacheManager = new DiskCacheManager();
	this->trxStore = nullptr;
}

TransactionContextCache::~TransactionContextCache() {
	close();

	delete this->baseDir;
	delete this->cacheManager;
}

void TransactionContextCache::init() {
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

void TransactionContextCache::open() {
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

void TransactionContextCache::close() {
	__close(&this->trxStore);
}

void TransactionContextCache::__close(Btree **tree) noexcept {
	if(*tree != nullptr){
		(*tree)->close();
		delete (*tree);
		(*tree) = nullptr;
	}
}

void TransactionContextCache::putTransaction(const AbstractBlockchainTransaction *trx) {
	const TransactionId* trxId = trx->getTransactionId();

	{
		TransactionIdKey key(trxId);
		TransactionData data(trx);

		this->trxStore->putData(&key, &data);
	}
}

bool TransactionContextCache::hasTransaction(const TransactionId *trxId) {
	TransactionIdKey key(trxId);

	IBlockObject* obj = this->trxStore->findByKey(&key); __STP(obj);
	return obj != nullptr;
}

BtreeScanner* TransactionContextCache::getBtreeScanner() const noexcept {
	return this->trxStore->getScanner();
}

} /* namespace codablecash */
