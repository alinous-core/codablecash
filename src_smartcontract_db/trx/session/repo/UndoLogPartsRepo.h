/*
 * UndoPartLogsRepo.h
 *
 *  Created on: 2023/07/09
 *      Author: iizuka
 */

#ifndef TRX_SESSION_REPO_UNDOLOGPARTSREPO_H_
#define TRX_SESSION_REPO_UNDOLOGPARTSREPO_H_

#include <cstdint>

namespace alinous {
class File;
class DiskCacheManager;
class Btree;
}
using namespace alinous;

namespace codablecash {

class UndoLogsPartIdKey;
class AbstractUndoLogPart;
class CdbDatabaseSessionId;
class UndoLogPartsScanner;

class UndoLogPartsRepo {
public:
	static const constexpr wchar_t* NAME{L"undologpart.bin"};

	explicit UndoLogPartsRepo(const File* undoDir);
	virtual ~UndoLogPartsRepo();

	void createRepository();

	void open();
	void close() noexcept;

	void addLogPart(const UndoLogsPartIdKey* partKey, const AbstractUndoLogPart *part);

	void rollback(const CdbDatabaseSessionId *sessionId, uint64_t currentUndoLogSerial, uint64_t partSerial);

	UndoLogPartsScanner* getScanner(const CdbDatabaseSessionId *sessionId, uint64_t undoLogSerial) const;

private:
	File* undoDir;
	DiskCacheManager* cacheManager;
	Btree* btree;
};

} /* namespace codablecash */

#endif /* TRX_SESSION_REPO_UNDOLOGPARTSREPO_H_ */
