/*
 * UndoAlterAddColumnLog.h
 *
 *  Created on: 2023/07/31
 *      Author: iizuka
 */

#ifndef TRX_SESSION_DDL_ALTER_UNDOALTERADDCOLUMNLOG_H_
#define TRX_SESSION_DDL_ALTER_UNDOALTERADDCOLUMNLOG_H_

#include "trx/session/base/AbstractUndoLog.h"

namespace codablecash {
class CdbTable;

class UndoAlterAddColumnLog : public AbstractUndoLog {
public:
	UndoAlterAddColumnLog(const UndoAlterAddColumnLog& inst);
	UndoAlterAddColumnLog();
	virtual ~UndoAlterAddColumnLog();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

	virtual void execute(CdbTransactionManager* trxManager, UndoLogPartsRepo* partsRepo, UndoLogsIdkey* key);

	void setTable(const CdbTable* table) noexcept;
	void setnewOid(uint64_t oid);

	int getRemoveColumnPos() const noexcept;

private:
	void handleParts(CdbTransactionManager *trxManager, UndoLogPartsRepo *partsRepo, UndoLogsIdkey* key);

private:
	CdbTable* table;
	uint64_t newOid;
};

} /* namespace codablecash */

#endif /* TRX_SESSION_DDL_ALTER_UNDOALTERADDCOLUMNLOG_H_ */
