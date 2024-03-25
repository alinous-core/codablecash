/*
 * UndoLogsRepo.cpp
 *
 *  Created on: 2023/07/09
 *      Author: iizuka
 */

#include "trx/session/repo/UndoLogsRepo.h"
#include "trx/session/repo/UndoLogsKeyFactory.h"
#include "trx/session/repo/UndoLogsDataFactory.h"
#include "trx/session/repo/UndoLogsIdkey.h"
#include "trx/session/repo/UndoLogsPartIdKey.h"
#include "trx/session/repo/UndoLogsRepoScanner.h"

#include "trx/session/base/AbstractUndoLog.h"
#include "trx/session/base/AbstractUndoLogPart.h"
#include "trx/session/base/CdbDatabaseSessionId.h"

#include "base_io/File.h"

#include "base/UnicodeString.h"
#include "base/StackRelease.h"

#include "btree/Btree.h"
#include "btree/BtreeConfig.h"

#include "random_access_file/DiskCacheManager.h"


namespace codablecash {

UndoLogsRepo::UndoLogsRepo(const File* undoDir) {
	this->undoDir = new File(*undoDir);
	this->cacheManager = new DiskCacheManager();
	this->btree = nullptr;
}

UndoLogsRepo::~UndoLogsRepo() {
	close();

	delete this->undoDir;
	delete this->cacheManager;
}

void UndoLogsRepo::createRepository() {
	UnicodeString fileName(NAME);

	UndoLogsKeyFactory* keyFactory = new UndoLogsKeyFactory(); __STP(keyFactory);
	UndoLogsDataFactory* dataFactory = new UndoLogsDataFactory(); __STP(dataFactory);

	Btree btree(this->undoDir, &fileName, this->cacheManager, keyFactory, dataFactory);

	BtreeConfig config;
	config.nodeNumber = 8;
	config.defaultSize = 1024;
	config.blockSize = 32;
	btree.create(&config);
}

void UndoLogsRepo::open() {
	close();

	UnicodeString fileName(NAME);

	UndoLogsKeyFactory* keyFactory = new UndoLogsKeyFactory(); __STP(keyFactory);
	UndoLogsDataFactory* dataFactory = new UndoLogsDataFactory(); __STP(dataFactory);

	this->btree = new Btree(this->undoDir, &fileName, this->cacheManager, keyFactory, dataFactory);

	BtreeOpenConfig opconf;
	opconf.numDataBuffer = 256;
	opconf.numNodeBuffer = 512;
	this->btree->open(&opconf);
}

void UndoLogsRepo::close() noexcept {
	if(this->btree != nullptr){
		this->btree->close();
		delete this->btree;
		this->btree = nullptr;
	}
}

void UndoLogsRepo::addLog(const UndoLogsIdkey *key, const AbstractUndoLog *log) {
	this->btree->putData(key, log);
}

UndoLogsRepoScanner* UndoLogsRepo::getScanner(const CdbDatabaseSessionId *sessionId) const {
	UndoLogsRepoScanner* scanner = new UndoLogsRepoScanner(this->btree, sessionId);
	scanner->init();

	return scanner;
}

void UndoLogsRepo::rollback(const CdbDatabaseSessionId *sessionId, uint64_t currentUndoLogSerial) {
	UndoLogsIdkey key;
	key.setSessionId(sessionId);
	key.setSerial(currentUndoLogSerial);

	this->btree->remove(&key);
}

} /* namespace codablecash */
