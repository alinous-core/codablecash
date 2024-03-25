/*
 * UndoSpaceManager.cpp
 *
 *  Created on: 2023/07/04
 *      Author: iizuka
 */

#include "trx/session/base/UndoSpaceManager.h"
#include "trx/session/base/AbstractUndoLog.h"

#include "trx/session/repo/UndoLogsRepo.h"
#include "trx/session/repo/UndoLogPartsRepo.h"
#include "trx/session/repo/UndoLogsIdkey.h"
#include "trx/session/repo/UndoLogsPartIdKey.h"
#include "trx/session/repo/UndoLogsRepoScanner.h"
#include "trx/session/repo/UndoLogsRepoSet.h"

#include "base/StackRelease.h"
#include "base/ArrayList.h"

#include "base_io/File.h"

#include "btree/Btree.h"

namespace codablecash {

UndoSpaceManager::UndoSpaceManager(const File* undoDir) {
	this->partSerial = 0;
	this->lastPartSerial = 0;
	this->currentUndoLogSerial = 0;
	this->undoDir = new File(*undoDir);
	this->repoSet = nullptr;
}

UndoSpaceManager::~UndoSpaceManager() {
	delete this->repoSet;
	delete this->undoDir;
}

void UndoSpaceManager::createUndoSpace(const File *undoDir) {
	undoDir->deleteDir();
	undoDir->mkdirs();
}

void UndoSpaceManager::newSession(const CdbDatabaseSessionId *sessionId) noexcept {
	UndoLogsRepoSet::createUndoSpace(this->undoDir, sessionId);
}

void UndoSpaceManager::addUndoLog(const CdbDatabaseSessionId *sessionId, uint64_t undoLogSerial, const AbstractUndoLog *log) {
	this->lastPartSerial = this->partSerial;
	this->partSerial = 0;
	this->currentUndoLogSerial = undoLogSerial;

	UndoLogsIdkey key;
	key.setSessionId(sessionId);
	key.setSerial(undoLogSerial);

	UndoLogsRepoSet* set = getRepoSet(sessionId);
	set->addLog(&key, log);
}

void UndoSpaceManager::addUndoLogPart(const CdbDatabaseSessionId *sessionId, uint64_t undoLogSerial, const AbstractUndoLogPart *part) {
	this->partSerial++;

	UndoLogsIdkey key;
	key.setSessionId(sessionId);
	key.setSerial(undoLogSerial);

	UndoLogsPartIdKey partKey;
	partKey.setLogIdKey(&key);
	partKey.setPartSerial(this->partSerial);

	UndoLogsRepoSet* set = getRepoSet(sessionId);
	set->addLogPart(&partKey, part);
}

void UndoSpaceManager::undoSession(const CdbDatabaseSessionId *sessionId, CdbTransactionManager *trxManager) noexcept {
	UndoLogsRepoSet* set = getRepoSet(sessionId);
	UndoLogsRepo* repo = set->getRepo();
	UndoLogPartsRepo* partsRepo = set->getPartRepo();

	ArrayList<AbstractUndoLog> list;
	ArrayList<UndoLogsIdkey> keylist;

	{
		UndoLogsRepoScanner* scanner = repo->getScanner(sessionId); __STP(scanner);
		while(scanner->hasNext()){
			const AbstractUndoLog* log = scanner->next();
			const UndoLogsIdkey* key = scanner->nextKey();

			list.addElement(dynamic_cast<AbstractUndoLog*>(log->copyData()));
			keylist.addElement(dynamic_cast<UndoLogsIdkey*>(key->copyData()));
		}
	}

	while(!list.isEmpty()){
		AbstractUndoLog* log = list.pop(); __STP(log);
		UndoLogsIdkey* key = keylist.pop(); __STP(key);

		log->execute(trxManager, partsRepo, key);
	}
}

UndoLogsRepoSet* UndoSpaceManager::getRepoSet(const CdbDatabaseSessionId *sessionId) {
	if(this->repoSet == nullptr || !this->repoSet->equals(sessionId)){
		delete this->repoSet;

		this->repoSet = new UndoLogsRepoSet(this->undoDir, sessionId);
		this->repoSet->load();
	}

	return this->repoSet;
}

void UndoSpaceManager::rollbackLastCommand() {
	this->repoSet->rollback(this->currentUndoLogSerial, this->partSerial);

	this->partSerial = this->lastPartSerial;
	this->lastPartSerial = 0;

	this->currentUndoLogSerial--;
}

} /* namespace codablecash */
