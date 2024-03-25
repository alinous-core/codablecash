/*
 * UndoLogPartsScanner.cpp
 *
 *  Created on: 2023/07/17
 *      Author: iizuka
 */

#include "trx/session/repo/UndoLogPartsScanner.h"
#include "trx/session/repo/UndoLogsIdkey.h"
#include "trx/session/repo/UndoLogsPartIdKey.h"

#include "trx/session/base/CdbDatabaseSessionId.h"
#include "trx/session/base/AbstractUndoLogPart.h"

#include "btree/Btree.h"
#include "btree/BtreeScanner.h"

#include "engine/CdbOid.h"

namespace codablecash {

UndoLogPartsScanner::UndoLogPartsScanner(Btree* btree, const CdbDatabaseSessionId *sessionId, uint64_t undoLogSerial) {
	this->btree = btree;
	this->sessionId = sessionId;
	this->undoLogSerial = undoLogSerial;
	this->scanner = nullptr;
	this->nextvalue = nullptr;
}

UndoLogPartsScanner::~UndoLogPartsScanner() {
	close();
	this->btree = nullptr;
	this->sessionId = nullptr;
	this->nextvalue = nullptr;
}

void UndoLogPartsScanner::close() noexcept {
	if(this->scanner != nullptr){
		delete this->scanner;
		this->scanner = nullptr;
	}
}

void UndoLogPartsScanner::init() {
	this->scanner = this->btree->getScanner();

	UndoLogsIdkey key;
	key.setSessionId(this->sessionId);
	key.setSerial(this->undoLogSerial);

	UndoLogsPartIdKey partKey;
	partKey.setLogIdKey(&key);
	partKey.setPartSerial(0L);

	this->scanner->begin(&partKey);
}

bool UndoLogPartsScanner::hasNext() {
	bool next = this->scanner->hasNext();
	if(!next){
		return false;
	}

	const IBlockObject* obj = this->scanner->next();
	this->nextvalue = dynamic_cast<const AbstractUndoLogPart*>(obj);

	const AbstractBtreeKey* nKey = this->scanner->nextKey();

	const UndoLogsPartIdKey* key = dynamic_cast<const UndoLogsPartIdKey*>(nKey);
	const CdbDatabaseSessionId* keySessionId = key->getSessionId();

	const CdbOid* serialOid = key->getUndoLogSerial();
	uint64_t serial = serialOid->getOidValue();

	return this->sessionId->compareTo(keySessionId) == 0 && this->undoLogSerial == serial;
}

const AbstractUndoLogPart* UndoLogPartsScanner::next() const noexcept {
	return this->nextvalue;
}

} /* namespace codablecash */
