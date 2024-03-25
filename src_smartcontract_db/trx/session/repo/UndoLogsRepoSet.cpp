/*
 * UndoLogsRepoSet.cpp
 *
 *  Created on: 2023/07/15
 *      Author: iizuka
 */

#include "trx/session/repo/UndoLogsRepoSet.h"
#include "trx/session/repo/UndoLogPartsRepo.h"
#include "trx/session/repo/UndoLogsRepo.h"

#include "trx/session/base/CdbDatabaseSessionId.h"

#include "base/UnicodeString.h"
#include "base/StackRelease.h"

#include "base_io/File.h"



namespace codablecash {

UndoLogsRepoSet::UndoLogsRepoSet(const File* undoDir, const CdbDatabaseSessionId *sessionId) {
	UnicodeString* str = sessionId->toString(); __STP(str);
	this->dir = undoDir->get(str);

	this->repo = nullptr;
	this->partsRepo = nullptr;

	this->sessionId = dynamic_cast<CdbDatabaseSessionId*>(sessionId->copyData());
}

UndoLogsRepoSet::~UndoLogsRepoSet() {
	delete this->repo;
	delete this->partsRepo;
	delete this->dir;
	delete this->sessionId;
}

void UndoLogsRepoSet::createUndoSpace(const File *undoDir, const CdbDatabaseSessionId *sessionId) {
	UnicodeString* str = sessionId->toString(); __STP(str);
	File* sessionUndoDir = undoDir->get(str); __STP(sessionUndoDir);

	sessionUndoDir->deleteDir();
	sessionUndoDir->mkdirs();

	UndoLogsRepo repo(sessionUndoDir);
	UndoLogPartsRepo partsRepo(sessionUndoDir);

	repo.createRepository();
	partsRepo.createRepository();
}

void UndoLogsRepoSet::load() {
	this->repo = new UndoLogsRepo(this->dir);
	this->partsRepo = new UndoLogPartsRepo(this->dir);

	this->repo->open();
	this->partsRepo->open();
}

bool UndoLogsRepoSet::equals(const CdbDatabaseSessionId *sessionId) const noexcept {
	return this->sessionId->equals(sessionId);
}

void UndoLogsRepoSet::addLog(const UndoLogsIdkey *key, const AbstractUndoLog *log) {
	this->repo->addLog(key, log);
}

void UndoLogsRepoSet::addLogPart(const UndoLogsPartIdKey *partKey, const AbstractUndoLogPart *part) {
	this->partsRepo->addLogPart(partKey, part);
}

void UndoLogsRepoSet::rollback(uint64_t currentUndoLogSerial, uint64_t partSerial) {
	this->partsRepo->rollback(this->sessionId, currentUndoLogSerial, partSerial);
	this->repo->rollback(this->sessionId, currentUndoLogSerial);
}

} /* namespace codablecash */
