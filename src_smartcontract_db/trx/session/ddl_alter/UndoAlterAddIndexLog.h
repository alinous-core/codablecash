/*
 * UndoAlterAddIndexLog.h
 *
 *  Created on: 2023/07/24
 *      Author: iizuka
 */

#ifndef TRX_SESSION_DDL_ALTER_UNDOALTERADDINDEXLOG_H_
#define TRX_SESSION_DDL_ALTER_UNDOALTERADDINDEXLOG_H_

#include "trx/session/base/AbstractUndoLog.h"

namespace codablecash {

class UndoAlterAddIndexLog : public AbstractUndoLog {
public:
	UndoAlterAddIndexLog(const UndoAlterAddIndexLog& inst);
	UndoAlterAddIndexLog();
	virtual ~UndoAlterAddIndexLog();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

	virtual void execute(CdbTransactionManager* trxManager, UndoLogPartsRepo* partsRepo, UndoLogsIdkey* key);


	void setNewIndexOid(uint64_t oid) noexcept;
	void setTableOid(uint64_t oid) noexcept;

private:
	uint64_t tableOid;
	uint64_t newIndexOid;
};

} /* namespace codablecash */

#endif /* TRX_SESSION_DDL_ALTER_UNDOALTERADDINDEXLOG_H_ */
