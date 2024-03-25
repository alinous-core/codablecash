/*
 * UndoLogsRepoScanner.cpp
 *
 *  Created on: 2023/07/12
 *      Author: iizuka
 */
#include "trx/session/repo/UndoLogsRepoScanner.h"
#include "trx/session/repo/UndoLogsIdkey.h"

#include "trx/session/base/AbstractUndoLog.h"
#include "trx/session/base/CdbDatabaseSessionId.h"

#include "btree/BtreeScanner.h"
#include "btree/Btree.h"


namespace codablecash {

UndoLogsRepoScanner::UndoLogsRepoScanner(Btree* btree, const CdbDatabaseSessionId *sessionId) {
	this->btree = btree;
	this->sessionId = sessionId;
	this->scanner = nullptr;
	this->nextvalue = nullptr;
	this->nextKeyValue = nullptr;
}


UndoLogsRepoScanner::~UndoLogsRepoScanner() {
	close();
	this->btree = nullptr;
	this->sessionId = nullptr;
}

void UndoLogsRepoScanner::init() {
	this->scanner = this->btree->getScanner();

	UndoLogsIdkey key;
	key.setSessionId(sessionId);
	key.setSerial(0);

	this->scanner->begin(&key);
}

void UndoLogsRepoScanner::close() noexcept {
	if(this->scanner != nullptr){
		delete this->scanner;
		this->scanner = nullptr;
	}
}

bool UndoLogsRepoScanner::hasNext() {
	bool next = this->scanner->hasNext();
	if(!next){
		return false;
	}

	const IBlockObject* obj = this->scanner->next();
	this->nextvalue = dynamic_cast<const AbstractUndoLog*>(obj);

	const AbstractBtreeKey* nKey = this->scanner->nextKey();

	this->nextKeyValue = dynamic_cast<const UndoLogsIdkey*>(nKey);
	const CdbDatabaseSessionId* keySessionId = this->nextKeyValue->getSessionId();

	return this->sessionId->compareTo(keySessionId) == 0;
}

const AbstractUndoLog* UndoLogsRepoScanner::next() const noexcept {
	return this->nextvalue;
}

const UndoLogsIdkey* UndoLogsRepoScanner::nextKey() const noexcept {
	return this->nextKeyValue;
}

} /* namespace codablecash */
