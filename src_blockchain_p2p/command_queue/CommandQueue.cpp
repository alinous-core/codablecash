/*
 * CommandQueue.cpp
 *
 *  Created on: 2023/09/19
 *      Author: iizuka
 */

#include "command_queue/CommandQueue.h"
#include "command_queue/CommandQueueDataFactory.h"
#include "command_queue/CommandQueueData.h"

#include "random_access_file/DiskCacheManager.h"

#include "btree/Btree.h"
#include "btree/BtreeConfig.h"
#include "btree/BtreeScanner.h"
#include "btree/BtreeReverseScanner.h"

#include "btreekey/BtreeKeyFactory.h"
#include "btreekey/ULongKey.h"

#include "base/StackRelease.h"
#include "base/UnicodeString.h"

#include "base_io/File.h"

#include "bc_p2p_cmd_node/AbstractNodeCommand.h"

#include "filestore_block/IBlockObject.h"


namespace codablecash {

CommandQueue::CommandQueue(const File* baseDir) {
	this->baseDir = baseDir->get(DIR_NAME);

	this->cacheManager = new DiskCacheManager();
	this->dataStore = nullptr;
	this->serial = 0;
}

CommandQueue::~CommandQueue() {
	close();

	delete this->baseDir;
	delete this->cacheManager;
}

void CommandQueue::close() {
	if(this->dataStore != nullptr){
		this->dataStore->close();
		delete this->dataStore;
		this->dataStore = nullptr;
	}
}

void CommandQueue::createBlankDatabase() {
	UnicodeString fileName(MAIN_DATA);

	BtreeKeyFactory* keyFactory = new BtreeKeyFactory(); __STP(keyFactory); // ULongKey
	CommandQueueDataFactory* dataFactory = new CommandQueueDataFactory(); __STP(dataFactory);

	Btree btree(this->baseDir, &fileName, this->cacheManager, keyFactory, dataFactory);

	BtreeConfig config;
	config.nodeNumber = 8;
	config.defaultSize = 1024;
	config.blockSize = 32;
	btree.create(&config);
}

void CommandQueue::open() {
	UnicodeString fileName(MAIN_DATA);

	BtreeKeyFactory* keyFactory = new BtreeKeyFactory(); __STP(keyFactory);
	CommandQueueDataFactory* dataFactory = new CommandQueueDataFactory(); __STP(dataFactory);

	this->dataStore = new Btree(this->baseDir, &fileName, this->cacheManager, keyFactory, dataFactory);
	BtreeOpenConfig opconf;
	opconf.numDataBuffer = 256;
	opconf.numNodeBuffer = 512;
	this->dataStore->open(&opconf);
}

void CommandQueue::addCommand(const CommandQueueData *cmd) {
	ULongKey key(this->serial++);

	this->dataStore->putData(&key, cmd);
}

CommandQueueData* CommandQueue::getFirst() const {
	CommandQueueData* ret = nullptr;
	BtreeScanner* scanner = this->dataStore->getScanner(); __STP(scanner);

	scanner->begin();

	if(scanner->hasNext()){
		const IBlockObject* obj = scanner->next();
		ret = dynamic_cast<CommandQueueData*>(obj->copyData());
	}

	return ret;
}

ULongKey* CommandQueue::getFirstKey() const {
	ULongKey* ret = nullptr;
	BtreeScanner* scanner = this->dataStore->getScanner(); __STP(scanner);

	scanner->begin();

	if(scanner->hasNext()){
		const AbstractBtreeKey* obj = scanner->nextKey();
		ret = dynamic_cast<ULongKey*>(obj->copyData());
	}

	return ret;
}

CommandQueueData* CommandQueue::getLast() const {
	CommandQueueData* ret = nullptr;
	BtreeReverseScanner* scanner = this->dataStore->getReverseScanner(); __STP(scanner);

	scanner->begin();

	if(scanner->hasPrevious()){
		const IBlockObject* obj = scanner->previous();
		ret = dynamic_cast<CommandQueueData*>(obj->copyData());
	}

	return ret;
}

bool CommandQueue::isEmpty() const {
	BtreeScanner* scanner = this->dataStore->getScanner(); __STP(scanner);
	scanner->begin();

	return !scanner->hasNext();
}

CommandQueueData* CommandQueue::fetchFirst() const {
	ULongKey* key = getFirstKey(); __STP(key);

	IBlockObject* obj = this->dataStore->findByKey(key);
	this->dataStore->remove(key);

	return dynamic_cast<CommandQueueData*>(obj);
}

} /* namespace codablecash */
