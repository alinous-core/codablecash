/*
 * UndoPartLogsRepo.cpp
 *
 *  Created on: 2023/07/09
 *      Author: iizuka
 */
#include "trx/session/repo/UndoLogPartsRepo.h"
#include "trx/session/repo/UndoLogsPartDataFactory.h"
#include "trx/session/repo/UndoLogsKeyFactory.h"
#include "trx/session/repo/UndoLogsPartIdKey.h"
#include "trx/session/repo/UndoLogsIdkey.h"

#include "trx/session/base/AbstractUndoLogPart.h"

#include "base/StackRelease.h"
#include "base/UnicodeString.h"

#include "base_io/File.h"

#include "btree/Btree.h"
#include "btree/BtreeConfig.h"

#include "random_access_file/DiskCacheManager.h"

#include "trx/session/repo/UndoLogPartsScanner.h"
namespace codablecash {

UndoLogPartsRepo::UndoLogPartsRepo(const File* undoDir) {
	this->undoDir = new File(*undoDir);
	this->cacheManager = new DiskCacheManager();
	this->btree = nullptr;
}

UndoLogPartsRepo::~UndoLogPartsRepo() {
	close();

	delete this->undoDir;
	delete this->cacheManager;
}

void UndoLogPartsRepo::createRepository() {
	UnicodeString fileName(NAME);

	UndoLogsKeyFactory* keyFactory = new UndoLogsKeyFactory(); __STP(keyFactory);
	UndoLogsPartDataFactory* dataFactory = new UndoLogsPartDataFactory(); __STP(dataFactory);

	Btree btree(this->undoDir, &fileName, this->cacheManager, keyFactory, dataFactory);

	BtreeConfig config;
	config.nodeNumber = 8;
	config.defaultSize = 1024;
	config.blockSize = 32;
	btree.create(&config);
}

void UndoLogPartsRepo::open() {
	close();

	UnicodeString fileName(NAME);

	UndoLogsKeyFactory* keyFactory = new UndoLogsKeyFactory(); __STP(keyFactory);
	UndoLogsPartDataFactory* dataFactory = new UndoLogsPartDataFactory(); __STP(dataFactory);

	this->btree = new Btree(this->undoDir, &fileName, this->cacheManager, keyFactory, dataFactory);

	BtreeOpenConfig opconf;
	opconf.numDataBuffer = 256;
	opconf.numNodeBuffer = 512;
	this->btree->open(&opconf);
}

void UndoLogPartsRepo::close() noexcept {
	if(this->btree != nullptr){
		this->btree->close();
		delete this->btree;
		this->btree = nullptr;
	}
}

void UndoLogPartsRepo::addLogPart(const UndoLogsPartIdKey *partKey, const AbstractUndoLogPart *part) {
	this->btree->putData(partKey, part);
}

void UndoLogPartsRepo::rollback(const CdbDatabaseSessionId *sessionId, uint64_t currentUndoLogSerial, uint64_t partSerial) {
	UndoLogsIdkey key;
	key.setSessionId(sessionId);
	key.setSerial(currentUndoLogSerial);

	uint64_t maxLoop = partSerial + 1;
	for(uint64_t i = 1; i != maxLoop; ++i){
		UndoLogsPartIdKey partKey;
		partKey.setLogIdKey(&key);
		partKey.setPartSerial(i);

		this->btree->remove(&partKey);
	}
}

UndoLogPartsScanner* UndoLogPartsRepo::getScanner(const CdbDatabaseSessionId *sessionId, uint64_t undoLogSerial) const {
	UndoLogPartsScanner* scanner = new UndoLogPartsScanner(this->btree, sessionId, undoLogSerial);
	scanner->init();

	return scanner;
}

} /* namespace codablecash */
