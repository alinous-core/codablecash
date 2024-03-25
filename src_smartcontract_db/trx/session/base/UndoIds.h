/*
 * UndoIds.h
 *
 *  Created on: 2023/07/09
 *      Author: iizuka
 */

#ifndef TRX_SESSION_BASE_UNDOIDS_H_
#define TRX_SESSION_BASE_UNDOIDS_H_

#include <cstdint>

#include "trx/session/base/AbstractUndoLog.h"

#include "snapshot/ISnapshotHandler.h"

namespace codablecash {

class CdbDatabaseSessionId;

class UndoIds : public AbstractUndoLog, public ISnapshotHandler {
public:
	UndoIds(const UndoIds& inst);
	UndoIds();
	virtual ~UndoIds();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

	virtual void putCommand(const AbstractSnapshotCommand* command);

	virtual void execute(CdbTransactionManager* trxManager, UndoLogPartsRepo* partsRepo, UndoLogsIdkey* key);

	void setLastSessionId(const CdbDatabaseSessionId* sessionId) noexcept;

private:
	CdbDatabaseSessionId* lastSessionId;

	uint64_t maxTransactionId;
	uint64_t maxObjectId;
	uint64_t maxSchemaObjectId;
	uint64_t maxCommitId;
};

} /* namespace codablecash */

#endif /* TRX_SESSION_BASE_UNDOIDS_H_ */
