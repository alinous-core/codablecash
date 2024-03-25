/*
 * UndoLogPartsScanner.h
 *
 *  Created on: 2023/07/17
 *      Author: iizuka
 */

#ifndef TRX_SESSION_REPO_UNDOLOGPARTSSCANNER_H_
#define TRX_SESSION_REPO_UNDOLOGPARTSSCANNER_H_

#include <cstdint>

namespace alinous {
class Btree;
class BtreeScanner;
}
using namespace alinous;

namespace codablecash {

class CdbDatabaseSessionId;
class AbstractUndoLogPart;

class UndoLogPartsScanner {
public:
	UndoLogPartsScanner(Btree* btree, const CdbDatabaseSessionId *sessionId, uint64_t undoLogSerial);
	virtual ~UndoLogPartsScanner();

	void init();

	void close() noexcept;

	bool hasNext();
	const AbstractUndoLogPart* next() const noexcept;

private:
	Btree* btree;
	const CdbDatabaseSessionId *sessionId;
	uint64_t undoLogSerial;

	BtreeScanner* scanner;
	const AbstractUndoLogPart* nextvalue;
};

} /* namespace codablecash */

#endif /* TRX_SESSION_REPO_UNDOLOGPARTSSCANNER_H_ */
