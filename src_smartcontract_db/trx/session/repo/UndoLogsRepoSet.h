/*
 * UndoLogsRepoSet.h
 *
 *  Created on: 2023/07/15
 *      Author: iizuka
 */

#ifndef TRX_SESSION_REPO_UNDOLOGSREPOSET_H_
#define TRX_SESSION_REPO_UNDOLOGSREPOSET_H_

#include <cstdint>

namespace alinous {
class File;
class Btree;
}
using namespace alinous;

namespace codablecash {

class CdbDatabaseSessionId;
class UndoLogsRepo;
class UndoLogPartsRepo;
class UndoLogsIdkey;
class AbstractUndoLog;
class UndoLogsPartIdKey;
class AbstractUndoLogPart;

class UndoLogsRepoSet {
public:
	UndoLogsRepoSet(const File* undoDir, const CdbDatabaseSessionId *sessionId);
	virtual ~UndoLogsRepoSet();

	static void createUndoSpace(const File* undoDir, const CdbDatabaseSessionId *sessionId);

	void load();

	bool equals(const CdbDatabaseSessionId* sessionId) const noexcept;

	void addLog(const UndoLogsIdkey* key, const AbstractUndoLog *log);
	void addLogPart(const UndoLogsPartIdKey* partKey, const AbstractUndoLogPart *part);

	UndoLogsRepo* getRepo() const noexcept {
		return this->repo;
	}
	UndoLogPartsRepo* getPartRepo() const noexcept {
		return this->partsRepo;
	}

	void rollback(uint64_t currentUndoLogSerial, uint64_t partSerial);

private:
	File* dir;
	CdbDatabaseSessionId* sessionId;

	UndoLogsRepo* repo;
	UndoLogPartsRepo* partsRepo;
};

} /* namespace codablecash */

#endif /* TRX_SESSION_REPO_UNDOLOGSREPOSET_H_ */
