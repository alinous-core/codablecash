/*
 * InstanceIdIndex.cpp
 *
 *  Created on: Nov 28, 2025
 *      Author: iizuka
 */

#include "smartcontract_executor_index/InstanceIdIndex.h"
#include "smartcontract_executor_index/InstanceIdIndexDataFactory.h"
#include "smartcontract_executor_index/InstanceIdIndexKey.h"
#include "smartcontract_executor_index/InstanceIdIndexKeyFactory.h"

#include "random_access_file/DiskCacheManager.h"

#include "btree/Btree.h"
#include "btree/BtreeConfig.h"

#include "base/StackRelease.h"

#include "base_io/File.h"


namespace codablecash {

InstanceIdIndex::InstanceIdIndex(const File* baseDir) {
	this->baseDir = baseDir->get(DIR_NAME);

	this->cacheManager = new DiskCacheManager();
	this->dataStore = nullptr;
}

InstanceIdIndex::~InstanceIdIndex() {
	close();

	delete this->baseDir;
	delete this->cacheManager;
}

void InstanceIdIndex::createBlankDatabase() {
	UnicodeString fileName(MAIN_DATA);

	InstanceIdIndexKeyFactory* keyFactory = new InstanceIdIndexKeyFactory(); __STP(keyFactory);
	InstanceIdIndexDataFactory* dataFactory = new InstanceIdIndexDataFactory(); __STP(dataFactory);

	Btree btree(this->baseDir, &fileName, this->cacheManager, keyFactory, dataFactory);

	BtreeConfig config;
	config.nodeNumber = 8;
	config.defaultSize = 1024;
	config.blockSize = 32;
	btree.create(&config);
}

void InstanceIdIndex::open() {
	UnicodeString fileName(MAIN_DATA);

	InstanceIdIndexKeyFactory* keyFactory = new InstanceIdIndexKeyFactory(); __STP(keyFactory);
	InstanceIdIndexDataFactory* dataFactory = new InstanceIdIndexDataFactory(); __STP(dataFactory);

	this->dataStore = new Btree(this->baseDir, &fileName, this->cacheManager, keyFactory, dataFactory);
	BtreeOpenConfig opconf;
	opconf.numDataBuffer = 256;
	opconf.numNodeBuffer = 512;
	this->dataStore->open(&opconf);
}

void InstanceIdIndex::close() {
	if(this->dataStore != nullptr){
		this->dataStore->close();
		delete this->dataStore;
		this->dataStore = nullptr;
	}
}

} /* namespace codablecash */
