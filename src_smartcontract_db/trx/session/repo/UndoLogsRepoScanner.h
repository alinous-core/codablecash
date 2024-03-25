/*
 * UndoLogsRepoScanner.h
 *
 *  Created on: 2023/07/12
 *      Author: iizuka
 */

#ifndef TRX_SESSION_REPO_UNDOLOGSREPOSCANNER_H_
#define TRX_SESSION_REPO_UNDOLOGSREPOSCANNER_H_

namespace alinous {
class Btree;
class BtreeScanner;
}
using namespace alinous;

namespace codablecash {

class CdbDatabaseSessionId;
class AbstractUndoLog;
class UndoLogsIdkey;


class UndoLogsRepoScanner {
public:
	UndoLogsRepoScanner(Btree* btree, const CdbDatabaseSessionId *sessionId);
	virtual ~UndoLogsRepoScanner();

	void init();

	void close() noexcept;

	bool hasNext();
	const AbstractUndoLog* next() const noexcept;
	const UndoLogsIdkey* nextKey() const noexcept;

private:
	Btree* btree;
	const CdbDatabaseSessionId *sessionId;
	BtreeScanner* scanner;

	const AbstractUndoLog* nextvalue;
	const UndoLogsIdkey* nextKeyValue;
};

} /* namespace codablecash */

#endif /* TRX_SESSION_REPO_UNDOLOGSREPOSCANNER_H_ */
