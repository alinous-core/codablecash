/*
 * DdbDatabaseSession.cpp
 *
 *  Created on: 2023/07/04
 *      Author: iizuka
 */
#include "trx/session/base/CdbDatabaseSession.h"
#include "trx/transaction_log/AbstractTransactionLog.h"

#include "trx/session/base/UndoSpaceManager.h"
#include "trx/session/base/CdbDatabaseSessionId.h"

namespace codablecash {

CdbDatabaseSession::CdbDatabaseSession(const File* undoDir) {
	this->committedCommands = new ArrayList<AbstractTransactionLog>();
	this->undoManager = new UndoSpaceManager(undoDir);
	this->sessionId = CdbDatabaseSessionId::createRandomId();
	this->undoLogSerial = 0;
}

CdbDatabaseSession::~CdbDatabaseSession() {
	clear();

	delete this->committedCommands;
	delete this->undoManager;

	delete this->sessionId;
}

void CdbDatabaseSession::load() {
	this->undoLogSerial = 0;
	this->undoManager->newSession(this->sessionId);
}

void CdbDatabaseSession::clear() noexcept {
	this->committedCommands->deleteElements();
	this->committedCommands->reset();
}

void CdbDatabaseSession::addcommittedCommand(	const AbstractTransactionLog *cmd) noexcept {
	this->committedCommands->addElement(cmd->copy());
}

void CdbDatabaseSession::newSession(const CdbDatabaseSessionId *sessionId) noexcept {
	clear();

	delete this->sessionId;
	this->sessionId = dynamic_cast<CdbDatabaseSessionId*>(sessionId->copyData());

	this->undoLogSerial = 0;

	this->undoManager->newSession(sessionId);
}

void CdbDatabaseSession::addUndoLog(const AbstractUndoLog *log) {
	this->undoLogSerial++;

	this->undoManager->addUndoLog(this->sessionId, this->undoLogSerial, log);
}

void CdbDatabaseSession::addUndoLogPart(const AbstractUndoLogPart *part) {
	this->undoManager->addUndoLogPart(this->sessionId, this->undoLogSerial, part);
}

void CdbDatabaseSession::undoSession(const CdbDatabaseSessionId *sessionId, CdbTransactionManager* trxManager) noexcept {
	this->undoManager->undoSession(sessionId, trxManager);
}

void CdbDatabaseSession::setSessionId(const CdbDatabaseSessionId *sessionId) noexcept {
	delete this->sessionId;
	this->sessionId = dynamic_cast<CdbDatabaseSessionId*>(sessionId->copyData());
}

} /* namespace codablecash */
