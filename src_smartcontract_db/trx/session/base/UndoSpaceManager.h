/*
 * UndoSpaceManager.h
 *
 *  Created on: 2023/07/04
 *      Author: iizuka
 */

#ifndef TRX_SESSION_BASE_UNDOSPACEMANAGER_H_
#define TRX_SESSION_BASE_UNDOSPACEMANAGER_H_

#include <cstdint>

namespace alinous {
class File;
class Btree;
}
using namespace alinous;

namespace codablecash {

class UndoLogsRepo;
class UndoLogPartsRepo;
class AbstractUndoLog;
class AbstractUndoLogPart;
class CdbDatabaseSessionId;
class CdbTransactionManager;
class UndoLogsRepoSet;

class UndoSpaceManager {
public:
	UndoSpaceManager(const File* undoDir);
	virtual ~UndoSpaceManager();

	static void createUndoSpace(const File* undoDir);

	void newSession(const CdbDatabaseSessionId* sessionId) noexcept;

	void addUndoLog(const CdbDatabaseSessionId* sessionId, uint64_t undoLogSerial, const AbstractUndoLog* log);
	void addUndoLogPart(const CdbDatabaseSessionId* sessionId, uint64_t undoLogSerial, const AbstractUndoLogPart* part);

	void undoSession(const CdbDatabaseSessionId* sessionId, CdbTransactionManager* trxManager) noexcept;

	void rollbackLastCommand();

private:
	UndoLogsRepoSet* getRepoSet(const CdbDatabaseSessionId* sessionId);

private:
	uint64_t partSerial;
	uint64_t lastPartSerial;
	uint64_t currentUndoLogSerial;

	File* undoDir;

	UndoLogsRepoSet* repoSet;
};

} /* namespace codablecash */

#endif /* TRX_SESSION_BASE_UNDOSPACEMANAGER_H_ */
