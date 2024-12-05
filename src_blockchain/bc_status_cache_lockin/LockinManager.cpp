/*
 * LockinManager.cpp
 *
 *  Created on: 2024/02/11
 *      Author: iizuka
 */

#include "bc_status_cache_lockin/LockinManager.h"
#include "bc_status_cache_lockin/LockInOperationDataFactory.h"
#include "bc_status_cache_lockin/AbstractLockinOperation.h"

#include "base/UnicodeString.h"
#include "base/StackRelease.h"

#include "base_io/File.h"

#include "random_access_file/DiskCacheManager.h"

#include "btree/BtreeConfig.h"
#include "btree/Btree.h"

#include "btreekey/BtreeKeyFactory.h"
#include "btreekey/ULongKey.h"

#include "bc_status_cache_lockin/LockInOperationData.h"

namespace codablecash {

LockinManager::LockinManager(const File* baseDir) {
	this->baseDir = baseDir->get(NAME_LOCKIN_DATA);
	this->cacheManager = new DiskCacheManager();
	this->lockinActionsIndex = nullptr;
	this->finalizingHeight = 0;
}

LockinManager::~LockinManager() {
	close();
	delete this->baseDir;
	delete this->cacheManager;
}

void LockinManager::initBlank() {
	UnicodeString filename(NAME_LOCKIN_DATA);

	BtreeKeyFactory* keyFactory = new BtreeKeyFactory(); __STP(keyFactory);
	LockInOperationDataFactory* dataFactory = new LockInOperationDataFactory(); __STP(dataFactory);

	Btree btree(this->baseDir, &filename, this->cacheManager, keyFactory, dataFactory);

	BtreeConfig config;
	config.nodeNumber = 8;
	config.defaultSize = 1024;
	config.blockSize = 32;
	btree.create(&config);
}

void LockinManager::open() {
	close();

	UnicodeString filename(NAME_LOCKIN_DATA);

	BtreeKeyFactory* keyFactory = new BtreeKeyFactory(); __STP(keyFactory);
	LockInOperationDataFactory* dataFactory = new LockInOperationDataFactory(); __STP(dataFactory);

	this->lockinActionsIndex = new Btree(this->baseDir, &filename, this->cacheManager, keyFactory, dataFactory);

	BtreeOpenConfig opconf;
	opconf.numDataBuffer = 256;
	opconf.numNodeBuffer = 512;
	this->lockinActionsIndex->open(&opconf);
}

void LockinManager::close() noexcept {
	if(this->lockinActionsIndex != nullptr){
		this->lockinActionsIndex->close();
		delete this->lockinActionsIndex;
		this->lockinActionsIndex = nullptr;
	}
}

void LockinManager::addOperation(uint64_t height, const AbstractLockinOperation *op) {
	ULongKey key(height);

	LockInOperationData* baseData = new LockInOperationData(); __STP(baseData);
	baseData->add(op);

	this->lockinActionsIndex->putData(&key, baseData);
}

LockInOperationData* LockinManager::getOperantions(uint64_t height) {
	ULongKey key(height);

	IBlockObject* obj = this->lockinActionsIndex->findByKey(&key); __STP(obj);
	LockInOperationData* data = dynamic_cast<LockInOperationData*>(__STP_MV(obj));

	return data;
}

} /* namespace codablecash */
