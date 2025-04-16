/*
 * PendingClientCommandsQueue.cpp
 *
 *  Created on: Mar 26, 2025
 *      Author: iizuka
 */

#include "bc_wallet_net_cmd_queue/PendingClientCommandsQueue.h"

#include "bc_wallet_net_cmd_queue/ClientCommandsQueueDataFactory.h"
#include "bc_wallet_net_cmd_queue/ClientCommandsQueueData.h"

#include "base_io/File.h"

#include "base/StackRelease.h"

#include "btree/Btree.h"
#include "btree/BtreeConfig.h"
#include "btree/BtreeScanner.h"
#include "btree/AbstractBtreeKey.h"

#include "btreekey/BtreeKeyFactory.h"
#include "btreekey/ULongKey.h"

#include "random_access_file/DiskCacheManager.h"


namespace codablecash {

PendingClientCommandsQueue::PendingClientCommandsQueue(const File* baseDir) {
	this->baseDir = baseDir->get(DIR_NAME);

	this->cacheManager = new DiskCacheManager();
	this->dataStore = nullptr;
	this->serial = 0;
}

PendingClientCommandsQueue::~PendingClientCommandsQueue() {
	close();

	delete this->baseDir;
	delete this->cacheManager;
}

void PendingClientCommandsQueue::close() {
	if(this->dataStore != nullptr){
		this->dataStore->close();
		delete this->dataStore;
		this->dataStore = nullptr;
	}
}

void PendingClientCommandsQueue::open() {
	UnicodeString fileName(MAIN_DATA);

	BtreeKeyFactory* keyFactory = new BtreeKeyFactory(); __STP(keyFactory);
	ClientCommandsQueueDataFactory* dataFactory = new ClientCommandsQueueDataFactory(); __STP(dataFactory);

	this->dataStore = new Btree(this->baseDir, &fileName, this->cacheManager, keyFactory, dataFactory);
	BtreeOpenConfig opconf;
	opconf.numDataBuffer = 256;
	opconf.numNodeBuffer = 512;
	this->dataStore->open(&opconf);
}

void PendingClientCommandsQueue::createBlankDatabase() {
	UnicodeString fileName(MAIN_DATA);

	BtreeKeyFactory* keyFactory = new BtreeKeyFactory(); __STP(keyFactory); // ULongKey
	ClientCommandsQueueDataFactory* dataFactory = new ClientCommandsQueueDataFactory(); __STP(dataFactory);

	Btree btree(this->baseDir, &fileName, this->cacheManager, keyFactory, dataFactory);

	BtreeConfig config;
	config.nodeNumber = 8;
	config.defaultSize = 1024;
	config.blockSize = 32;
	btree.create(&config);
}

bool PendingClientCommandsQueue::isEmpty() const noexcept {
	BtreeScanner* scanner = this->dataStore->getScanner(); __STP(scanner);
	scanner->begin();

	return !scanner->hasNext();
}

ULongKey* PendingClientCommandsQueue::getFirstKey() const {
	ULongKey* ret = nullptr;
	BtreeScanner* scanner = this->dataStore->getScanner(); __STP(scanner);

	scanner->begin();

	if(scanner->hasNext()){
		const AbstractBtreeKey* obj = scanner->nextKey();
		ret = dynamic_cast<ULongKey*>(obj->copyData());
	}

	return ret;
}

ClientCommandsQueueData* PendingClientCommandsQueue::fetchFirst() const {
	ULongKey* key = getFirstKey(); __STP(key);

	IBlockObject* obj = this->dataStore->findByKey(key);
	this->dataStore->remove(key);

	return dynamic_cast<ClientCommandsQueueData*>(obj);
}

void PendingClientCommandsQueue::addCommnadData(const ClientCommandsQueueData *queueData) {
	ULongKey key(this->serial++);

	this->dataStore->putData(&key, queueData);
}

} /* namespace codablecash */
