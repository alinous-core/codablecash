/*
 * PendingProcessorCommandsQueue.cpp
 *
 *  Created on: 2023/11/10
 *      Author: iizuka
 */

#include "bc_p2p_processor_pending/PendingProcessorCommandsQueue.h"
#include "bc_p2p_processor_pending/PendingCommandData.h"
#include "bc_p2p_processor_pending/PendingCommandDataFactory.h"

#include "random_access_file/DiskCacheManager.h"

#include "base/StackRelease.h"

#include "base_io/File.h"

#include "btree/BtreeConfig.h"
#include "btree/Btree.h"

#include "btreekey/BtreeKeyFactory.h"
#include "btreekey/ULongKey.h"

#include "btree/BtreeScanner.h"

namespace codablecash {

PendingProcessorCommandsQueue::PendingProcessorCommandsQueue(const File* baseDir) {
	this->baseDir = baseDir->get(DIR_NAME);

	this->cacheManager = new DiskCacheManager();
	this->dataStore = nullptr;
	this->serial = 0;
}

PendingProcessorCommandsQueue::~PendingProcessorCommandsQueue() {
	close();

	delete this->baseDir;
	delete this->cacheManager;
}

void PendingProcessorCommandsQueue::createBlankDatabase() {
	UnicodeString fileName(MAIN_DATA);

	BtreeKeyFactory* keyFactory = new BtreeKeyFactory(); __STP(keyFactory); // ULongKey
	PendingCommandDataFactory* dataFactory = new PendingCommandDataFactory(); __STP(dataFactory);

	Btree btree(this->baseDir, &fileName, this->cacheManager, keyFactory, dataFactory);

	BtreeConfig config;
	config.nodeNumber = 8;
	config.defaultSize = 1024;
	config.blockSize = 32;
	btree.create(&config);
}

void PendingProcessorCommandsQueue::open() {
	// reset
	this->baseDir->deleteDir();
	this->baseDir->mkdirs();
	createBlankDatabase();

	UnicodeString fileName(MAIN_DATA);

	BtreeKeyFactory* keyFactory = new BtreeKeyFactory(); __STP(keyFactory);
	PendingCommandDataFactory* dataFactory = new PendingCommandDataFactory(); __STP(dataFactory);

	this->dataStore = new Btree(this->baseDir, &fileName, this->cacheManager, keyFactory, dataFactory);
	BtreeOpenConfig opconf;
	opconf.numDataBuffer = 256;
	opconf.numNodeBuffer = 512;
	this->dataStore->open(&opconf);
}

void PendingProcessorCommandsQueue::close() {
	if(this->dataStore != nullptr){
		this->dataStore->close();
		delete this->dataStore;
		this->dataStore = nullptr;
	}
}

void PendingProcessorCommandsQueue::put(const PendingCommandData *data) {
	ULongKey key(this->serial++);

	this->dataStore->putData(&key, data);
}

bool PendingProcessorCommandsQueue::isEmpty() const {
	BtreeScanner* scanner = this->dataStore->getScanner(); __STP(scanner);
	scanner->begin();

	return !scanner->hasNext();
}

ULongKey* PendingProcessorCommandsQueue::getFirstKey() const {
	ULongKey* ret = nullptr;
	BtreeScanner* scanner = this->dataStore->getScanner(); __STP(scanner);

	scanner->begin();

	if(scanner->hasNext()){
		const AbstractBtreeKey* obj = scanner->nextKey();
		ret = dynamic_cast<ULongKey*>(obj->copyData());
	}

	return ret;
}

PendingCommandData* PendingProcessorCommandsQueue::fetchFirst() const {
	ULongKey* key = getFirstKey(); __STP(key);

	IBlockObject* obj = this->dataStore->findByKey(key);
	this->dataStore->remove(key);

	return dynamic_cast<PendingCommandData*>(obj);
}

} /* namespace codablecash */
