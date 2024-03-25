/*
 * UndoAlterRenameColumn.h
 *
 *  Created on: 2023/08/04
 *      Author: iizuka
 */

#ifndef TRX_SESSION_DDL_ALTER_UNDOALTERRENAMECOLUMNLOG_H_
#define TRX_SESSION_DDL_ALTER_UNDOALTERRENAMECOLUMNLOG_H_

#include "trx/session/base/AbstractUndoLog.h"

namespace codablecash {

class CdbTable;

class UndoAlterRenameColumnLog : public AbstractUndoLog {
public:
	UndoAlterRenameColumnLog(const UndoAlterRenameColumnLog& inst);
	UndoAlterRenameColumnLog();
	virtual ~UndoAlterRenameColumnLog();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

	virtual void execute(CdbTransactionManager* trxManager, UndoLogPartsRepo* partsRepo, UndoLogsIdkey* key);

	void setTable(const CdbTable* tbl) noexcept;
	void setLastName(const UnicodeString* name) noexcept;
	void setNewName(const UnicodeString* name) noexcept;

private:
	CdbTable* table;
	UnicodeString* lastName;
	UnicodeString* newName;
};

} /* namespace codablecash */

#endif /* TRX_SESSION_DDL_ALTER_UNDOALTERRENAMECOLUMNLOG_H_ */
