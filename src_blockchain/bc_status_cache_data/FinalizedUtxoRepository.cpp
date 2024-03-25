/*
 * FinalizedUtxoRepository.cpp
 *
 *  Created on: 2023/05/08
 *      Author: iizuka
 */

#include "bc_status_cache_data/FinalizedUtxoRepository.h"

#include "base_io/File.h"

#include "random_access_file/DiskCacheManager.h"

#include "base/StackRelease.h"
#include "base/UnicodeString.h"

#include "btree/Btree.h"
#include "btree/BtreeConfig.h"

#include "bc_base_utxo_index/UtxoIdKeyFactory.h"
#include "bc_base_utxo_index/UtxoDataFactory.h"
#include "bc_base_utxo_index/UtxoIdKey.h"
#include "bc_base_utxo_index/UtxoData.h"

#include "bc_trx/AbstractUtxo.h"

#include "bc_block/BlockHeader.h"


namespace codablecash {

FinalizedUtxoRepository::FinalizedUtxoRepository(const File* baseDir) {
	this->baseDir = new File(*baseDir);
	this->cacheManager = new DiskCacheManager();
	this->utxoIndex = nullptr;
}

FinalizedUtxoRepository::~FinalizedUtxoRepository() {
	close();

	delete this->baseDir;
	delete this->cacheManager;
}

void FinalizedUtxoRepository::initBlank() {
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

void FinalizedUtxoRepository::open() {
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

void FinalizedUtxoRepository::close() {
	if(this->utxoIndex != nullptr){
		this->utxoIndex->close();
		delete this->utxoIndex, this->utxoIndex = nullptr;
	}
}

bool FinalizedUtxoRepository::remove(const UtxoId *utxoId) {
	UtxoIdKey key(utxoId);

	return this->utxoIndex->remove(&key);
}

void FinalizedUtxoRepository::put(const AbstractUtxo *utxo, const TransactionId* trxId, const BlockHeader* header) {
	uint64_t height = header->getHeight();
	const BlockHeaderId* headerId = header->getId();

	UtxoIdKey key(utxo->getId());
	UtxoData data(utxo, trxId, height, headerId);

	this->utxoIndex->putData(&key, &data);
}

UtxoData* FinalizedUtxoRepository::find(const UtxoId *utxoId) const {
	UtxoIdKey key(utxoId);

	IBlockObject* obj = this->utxoIndex->findByKey(&key); __STP(obj);
	UtxoData* utxo = dynamic_cast<UtxoData*>(obj);

	if(utxo == nullptr){
		return nullptr;
	}

	__STP_MV(obj);
	return utxo;
}

} /* namespace codablecash */
