/*
 * UndoLogsRepo.h
 *
 *  Created on: 2023/07/09
 *      Author: iizuka
 */

#ifndef TRX_SESSION_REPO_UNDOLOGSREPO_H_
#define TRX_SESSION_REPO_UNDOLOGSREPO_H_

#include <cstdint>

namespace alinous {
class File;
class DiskCacheManager;
class Btree;
}
using namespace alinous;

namespace codablecash {

class UndoLogsIdkey;
class AbstractUndoLog;
class UndoLogsPartIdKey;
class AbstractUndoLogPart;
class UndoLogsRepoScanner;
class CdbDatabaseSessionId;

class UndoLogsRepo {
public:
	static const constexpr wchar_t* NAME{L"undologs.bin"};

	explicit UndoLogsRepo(const File* undoDir);
	virtual ~UndoLogsRepo();

	void createRepository();

	void open();
	void close() noexcept;

	void addLog(const UndoLogsIdkey* key, const AbstractUndoLog *log);

	UndoLogsRepoScanner* getScanner(const CdbDatabaseSessionId *sessionId) const;

	void rollback(const CdbDatabaseSessionId *sessionId, uint64_t currentUndoLogSerial);

private:
	File* undoDir;
	DiskCacheManager* cacheManager;
	Btree* btree;
};

} /* namespace codablecash */

#endif /* TRX_SESSION_REPO_UNDOLOGSREPO_H_ */
