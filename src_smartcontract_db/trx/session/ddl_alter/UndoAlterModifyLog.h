/*
 * UndoAlterModifyLog.h
 *
 *  Created on: 2023/07/24
 *      Author: iizuka
 */

#ifndef TRX_SESSION_DDL_ALTER_UNDOALTERMODIFYLOG_H_
#define TRX_SESSION_DDL_ALTER_UNDOALTERMODIFYLOG_H_

#include "trx/session/base/AbstractUndoLog.h"

namespace codablecash {

class CdbTableColumn;

class UndoAlterModifyLog : public AbstractUndoLog {
public:
	UndoAlterModifyLog(const UndoAlterModifyLog& inst);
	UndoAlterModifyLog();
	virtual ~UndoAlterModifyLog();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

	virtual void execute(CdbTransactionManager* trxManager, UndoLogPartsRepo* partsRepo, UndoLogsIdkey* key);

	void setTableOid(uint64_t oid) noexcept;
	void setColumn(const CdbTableColumn* col) noexcept;
	void setDataModified(bool bl) noexcept;

private:
	void handleParts(CdbTransactionManager *trxManager, UndoLogPartsRepo *partsRepo, UndoLogsIdkey* key);

private:
	uint64_t tableOid;
	CdbTableColumn* column;

	bool dataModified;
};

} /* namespace codablecash */

#endif /* TRX_SESSION_DDL_ALTER_UNDOALTERMODIFYLOG_H_ */
