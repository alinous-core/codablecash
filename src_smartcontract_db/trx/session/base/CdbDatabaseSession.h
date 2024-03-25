/*
 * DdbDatabaseSession.h
 *
 *  Created on: 2023/07/04
 *      Author: iizuka
 */

#ifndef TRX_SESSION_DDBDATABASESESSION_H_
#define TRX_SESSION_DDBDATABASESESSION_H_

#include "base/ArrayList.h"

#include <cstdint>

namespace alinous {
class File;
}
using namespace alinous;

namespace codablecash {

class CdbDatabaseSessionId;
class AbstractTransactionLog;
class UndoSpaceManager;
class AbstractUndoLog;
class AbstractUndoLogPart;
class CdbTransactionManager;

class CdbDatabaseSession {
public:
	explicit CdbDatabaseSession(const File* undoDir);
	virtual ~CdbDatabaseSession();

	void load();
	void clear() noexcept;

	void newSession(const CdbDatabaseSessionId* sessionId) noexcept;
	void setSessionId(const CdbDatabaseSessionId* sessionId) noexcept;

	void addcommittedCommand(const AbstractTransactionLog* cmd) noexcept;

	void addUndoLog(const AbstractUndoLog* log);
	void addUndoLogPart(const AbstractUndoLogPart* part);

	const CdbDatabaseSessionId* getSessionId() const noexcept {
		return this->sessionId;
	}

	void undoSession(const CdbDatabaseSessionId* sessionId, CdbTransactionManager* trxManager) noexcept;

	UndoSpaceManager* getUndoSpaceManager() const noexcept {
		return this->undoManager;
	}

private:
	CdbDatabaseSessionId* sessionId;
	uint64_t undoLogSerial;

	ArrayList<AbstractTransactionLog>* committedCommands;

	UndoSpaceManager* undoManager;
};

} /* namespace codablecash */

#endif /* TRX_SESSION_DDBDATABASESESSION_H_ */
