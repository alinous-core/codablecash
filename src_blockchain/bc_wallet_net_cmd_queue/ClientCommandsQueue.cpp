/*
 * ClientCommandsQueue.cpp
 *
 *  Created on: Mar 26, 2025
 *      Author: iizuka
 */

#include "bc_wallet_net_cmd_queue/ClientCommandsQueue.h"

#include "bc_wallet_net_cmd/AbstractClientCommand.h"

#include "bc_wallet_net_cmd_queue/ClientCommandsQueueFactory.h"

#include "random_access_file/DiskCacheManager.h"

#include "base/StackRelease.h"
#include "base/UnicodeString.h"

#include "base_io/File.h"

#include "btree/Btree.h"
#include "btree/BtreeConfig.h"
#include "btree/BtreeScanner.h"
#include "btree/BtreeReverseScanner.h"

#include "btreekey/BtreeKeyFactory.h"
#include "btreekey/ULongKey.h"


namespace codablecash {

ClientCommandsQueue::ClientCommandsQueue(const File* baseDir) {
	this->baseDir = baseDir->get(DIR_NAME);

	this->cacheManager = new DiskCacheManager();
	this->dataStore = nullptr;
	this->serial = 0;
}

ClientCommandsQueue::~ClientCommandsQueue() {
	close();

	delete this->baseDir;
	delete this->cacheManager;
}

void ClientCommandsQueue::createBlankDatabase() {
	UnicodeString fileName(MAIN_DATA);

	BtreeKeyFactory* keyFactory = new BtreeKeyFactory(); __STP(keyFactory); // ULongKey
	ClientCommandsQueueFactory* dataFactory = new ClientCommandsQueueFactory(); __STP(dataFactory);

	Btree btree(this->baseDir, &fileName, this->cacheManager, keyFactory, dataFactory);

	BtreeConfig config;
	config.nodeNumber = 8;
	config.defaultSize = 1024;
	config.blockSize = 32;
	btree.create(&config);
}

void ClientCommandsQueue::open() {
	UnicodeString fileName(MAIN_DATA);

	BtreeKeyFactory* keyFactory = new BtreeKeyFactory(); __STP(keyFactory);
	ClientCommandsQueueFactory* dataFactory = new ClientCommandsQueueFactory(); __STP(dataFactory);

	this->dataStore = new Btree(this->baseDir, &fileName, this->cacheManager, keyFactory, dataFactory);
	BtreeOpenConfig opconf;
	opconf.numDataBuffer = 256;
	opconf.numNodeBuffer = 512;
	this->dataStore->open(&opconf);
}

void ClientCommandsQueue::close() {
	if(this->dataStore != nullptr){
		this->dataStore->close();
		delete this->dataStore;
		this->dataStore = nullptr;
	}
}

void ClientCommandsQueue::addCommand(const AbstractClientCommand *cmd) {
	ULongKey key(this->serial++);

	this->dataStore->putData(&key, cmd);
}

AbstractClientCommand* ClientCommandsQueue::getFirst() const {
	AbstractClientCommand* ret = nullptr;
	BtreeScanner* scanner = this->dataStore->getScanner(); __STP(scanner);

	scanner->begin();

	if(scanner->hasNext()){
		const IBlockObject* obj = scanner->next();
		ret = dynamic_cast<AbstractClientCommand*>(obj->copyData());
	}

	return ret;
}

ULongKey* ClientCommandsQueue::getFirstKey() const {
	ULongKey* ret = nullptr;
	BtreeScanner* scanner = this->dataStore->getScanner(); __STP(scanner);

	scanner->begin();

	if(scanner->hasNext()){
		const AbstractBtreeKey* obj = scanner->nextKey();
		ret = dynamic_cast<ULongKey*>(obj->copyData());
	}

	return ret;
}

AbstractClientCommand* ClientCommandsQueue::getLast() const {
	AbstractClientCommand* ret = nullptr;
	BtreeReverseScanner* scanner = this->dataStore->getReverseScanner(); __STP(scanner);

	scanner->begin();

	if(scanner->hasPrevious()){
		const IBlockObject* obj = scanner->previous();
		ret = dynamic_cast<AbstractClientCommand*>(obj->copyData());
	}

	return ret;
}

bool ClientCommandsQueue::isEmpty() const {
	BtreeScanner* scanner = this->dataStore->getScanner(); __STP(scanner);
	scanner->begin();

	return !scanner->hasNext();
}

AbstractClientCommand* ClientCommandsQueue::fetchFirst() const {
	ULongKey* key = getFirstKey(); __STP(key);

	IBlockObject* obj = this->dataStore->findByKey(key);
	this->dataStore->remove(key);

	return dynamic_cast<AbstractClientCommand*>(obj);
}

} /* namespace codablecash */
