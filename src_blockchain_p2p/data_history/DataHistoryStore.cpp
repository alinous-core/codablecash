/*
 * DataHistoryStore.cpp
 *
 *  Created on: 2023/09/13
 *      Author: iizuka
 */

#include "data_history/DataHistoryStore.h"
#include "data_history/TransferedDataIdKeyFactory.h"
#include "data_history/TransferedDataFactory.h"
#include "data_history/TimeIndexKeyFactory.h"
#include "data_history/TransferedDataIdListFactory.h"
#include "data_history/TransferedDataIdKey.h"
#include "data_history/TransferedDataIdList.h"
#include "data_history/TimeIndexKey.h"
#include "data_history/TransferedDataId.h"
#include "data_history/AbstractTransferedData.h"

#include "base_io/File.h"

#include "random_access_file/DiskCacheManager.h"

#include "btree/Btree.h"
#include "btree/BtreeConfig.h"

#include "base/UnicodeString.h"
#include "base/StackRelease.h"


namespace codablecash {

DataHistoryStore::DataHistoryStore(const File* baseDir) {
	this->baseDir = baseDir->get(DIR_NAME);

	this->cacheManager = new DiskCacheManager();
	this->dataStore = nullptr;
	this->timeIndex = nullptr;
}

DataHistoryStore::~DataHistoryStore() {
	close();

	delete this->baseDir;
	delete this->cacheManager;
}

void DataHistoryStore::createBlankDatabase() {
	{
		UnicodeString fileName(MAIN_DATA);

		TransferedDataIdKeyFactory* keyFactory = new TransferedDataIdKeyFactory(); __STP(keyFactory);
		TransferedDataFactory* dataFactory = new TransferedDataFactory(); __STP(dataFactory);

		Btree btree(this->baseDir, &fileName, this->cacheManager, keyFactory, dataFactory);

		BtreeConfig config;
		config.nodeNumber = 8;
		config.defaultSize = 1024;
		config.blockSize = 32;
		btree.create(&config);
	}

	{
		UnicodeString fileName(TIMEINDEX_DATA);

		TimeIndexKeyFactory* keyFactory = new TimeIndexKeyFactory(); __STP(keyFactory);
		TransferedDataIdListFactory* dataFactory = new TransferedDataIdListFactory(); __STP(dataFactory);

		Btree btree(this->baseDir, &fileName, this->cacheManager, keyFactory, dataFactory);

		BtreeConfig config;
		config.nodeNumber = 8;
		config.defaultSize = 1024;
		config.blockSize = 32;
		btree.create(&config);
	}
}

void DataHistoryStore::open() {
	{
		UnicodeString fileName(MAIN_DATA);

		TransferedDataIdKeyFactory* keyFactory = new TransferedDataIdKeyFactory(); __STP(keyFactory);
		TransferedDataFactory* dataFactory = new TransferedDataFactory(); __STP(dataFactory);

		this->dataStore = new Btree(this->baseDir, &fileName, this->cacheManager, keyFactory, dataFactory);
		BtreeOpenConfig opconf;
		opconf.numDataBuffer = 256;
		opconf.numNodeBuffer = 512;
		this->dataStore->open(&opconf);
	}

	{
		UnicodeString fileName(TIMEINDEX_DATA);

		TimeIndexKeyFactory* keyFactory = new TimeIndexKeyFactory(); __STP(keyFactory);
		TransferedDataIdListFactory* dataFactory = new TransferedDataIdListFactory(); __STP(dataFactory);

		this->timeIndex = new Btree(this->baseDir, &fileName, this->cacheManager, keyFactory, dataFactory);
		BtreeOpenConfig opconf;
		opconf.numDataBuffer = 256;
		opconf.numNodeBuffer = 512;
		this->timeIndex->open(&opconf);
	}
}

void DataHistoryStore::close() {
	if(this->dataStore != nullptr){
		this->dataStore->close();
		delete this->dataStore;
		this->dataStore = nullptr;
	}
	if(this->timeIndex != nullptr){
		this->timeIndex->close();
		delete this->timeIndex;
		this->timeIndex = nullptr;
	}
}

void DataHistoryStore::add(const TransferedDataId *dataId, const AbstractTransferedData* data) {
	{
		TransferedDataIdKey key(dataId);

		this->dataStore->putData(&key, data);
	}
	{
		TimeIndexKey key;
		TransferedDataIdList dataList;
		dataList.add(dataId);

		this->timeIndex->putData(&key, &dataList);
	}
}

bool DataHistoryStore::hasId(const TransferedDataId *dataId) const {
	TransferedDataIdKey key(dataId);

	IBlockObject* obj = this->dataStore->findByKey(&key); __STP(obj);
	return obj != nullptr;
}

} /* namespace codablecash */
