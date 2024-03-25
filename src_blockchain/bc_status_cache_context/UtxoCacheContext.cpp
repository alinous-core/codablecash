/*
 * UtxoCacheContext.cpp
 *
 *  Created on: 2023/04/25
 *      Author: iizuka
 */

#include "bc_status_cache_context/UtxoCacheContext.h"

#include "base_io/File.h"

#include "random_access_file/DiskCacheManager.h"

#include "btree/Btree.h"
#include "btree/BtreeConfig.h"

#include "base/StackRelease.h"

#include "bc_base_utxo_index/UtxoIdKeyFactory.h"
#include "bc_base_utxo_index/UtxoIdKey.h"

#include "bc_trx/AbstractUtxo.h"
#include "bc_trx/AbstractBlockchainTransaction.h"

#include "bc_base_utxo_index/UtxoDataFactory.h"
#include "bc_base_utxo_index/UtxoData.h"

#include "bc_block/BlockHeader.h"

#include "bc_status_cache_context/RemovedDummyUtxo.h"


namespace codablecash {

UtxoCacheContext::UtxoCacheContext(const File* baseDir) {
	this->baseDir = new File(*baseDir);
	this->cacheManager = new DiskCacheManager();
	this->utxoIndex = nullptr;
}

UtxoCacheContext::~UtxoCacheContext() {
	close();

	delete this->baseDir;
	delete this->cacheManager;
}

void UtxoCacheContext::init() {
	{
		UnicodeString fileName(NAME_UTXO_INDEX);

		UtxoIdKeyFactory* keyFactory = new UtxoIdKeyFactory(); __STP(keyFactory);
		UtxoDataFactory* dataFactory = new UtxoDataFactory(); __STP(dataFactory);

		Btree btree(this->baseDir, &fileName, this->cacheManager, keyFactory, dataFactory);

		BtreeConfig config;
		config.nodeNumber = 8;
		config.defaultSize = 1024;
		config.blockSize = 32;
		btree.create(&config);
	}
}

void UtxoCacheContext::open() {
	{
		UnicodeString fileName(NAME_UTXO_INDEX);

		UtxoIdKeyFactory* keyFactory = new UtxoIdKeyFactory(); __STP(keyFactory);
		UtxoDataFactory* dataFactory = new UtxoDataFactory(); __STP(dataFactory);

		this->utxoIndex = new Btree(this->baseDir, &fileName, this->cacheManager, keyFactory, dataFactory);
		BtreeOpenConfig opconf;
		opconf.numDataBuffer = 256;
		opconf.numNodeBuffer = 512;
		this->utxoIndex->open(&opconf);
	}
}

void UtxoCacheContext::close() noexcept {
	__close(&this->utxoIndex);
}

void UtxoCacheContext::__close(Btree **tree) noexcept {
	if(*tree != nullptr){
		(*tree)->close();
		delete (*tree);
		(*tree) = nullptr;
	}
}

UtxoData* UtxoCacheContext::getData(const UtxoId *utxoId) {
	UtxoIdKey key(utxoId);

	IBlockObject* obj = this->utxoIndex->findByKey(&key);
	return obj != nullptr ? dynamic_cast<UtxoData*>(obj) : nullptr;
}

void UtxoCacheContext::putUtxo(const AbstractUtxo *utxo, const TransactionId* trxId, uint64_t height) {
	UtxoIdKey key(utxo->getId());
	UtxoData data(utxo, trxId, height, nullptr);

	this->utxoIndex->putData(&key, &data);
}

void UtxoCacheContext::putUtxo(const UtxoData *data) {
	const AbstractUtxo* utxo = data->getUtxo();
	UtxoIdKey key(utxo->getId());

	this->utxoIndex->putData(&key, data);
}

bool UtxoCacheContext::removeUtxo(const UtxoId *utxoId, const AbstractBlockchainTransaction *trx) {
	bool ret = false;

	UtxoData* data = getData(utxoId);
	if(data == nullptr){
		// put dummy data with removed flag true
		RemovedDummyUtxo dutxo;
		dutxo.setUtxoId(utxoId);

		const TransactionId* trxId = trx->getTransactionId();

		data = new UtxoData(&dutxo, trxId, 0, nullptr);
	}
	else{
		assert(data->isRemoved() == false);
		ret = true;
	}
	 __STP(data);

	 data->setRemoved(true);

	 {
		 UtxoIdKey key(utxoId);
		 this->utxoIndex->putData(&key, data);
	 }

	return ret;
}

BtreeScanner* UtxoCacheContext::getBtreeScanner() const noexcept {
	return this->utxoIndex->getScanner();
}

} /* namespace codablecash */
