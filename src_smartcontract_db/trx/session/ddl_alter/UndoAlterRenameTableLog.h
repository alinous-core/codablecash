/*
 * UndoAlterRenameTableLog.h
 *
 *  Created on: 2023/07/05
 *      Author: iizuka
 */

#ifndef TRX_SESSION_DDL_ALTER_UNDOALTERRENAMETABLELOG_H_
#define TRX_SESSION_DDL_ALTER_UNDOALTERRENAMETABLELOG_H_

#include "trx/session/base/AbstractUndoLog.h"

namespace codablecash {

class TableRenameContext;

class UndoAlterRenameTableLog : public AbstractUndoLog {
public:
	UndoAlterRenameTableLog(const UndoAlterRenameTableLog& inst);
	UndoAlterRenameTableLog();
	virtual ~UndoAlterRenameTableLog();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

	virtual void execute(CdbTransactionManager* trxManager, UndoLogPartsRepo* partsRepo, UndoLogsIdkey* key);

	void importFromContext(const TableRenameContext* context) noexcept;

private:
	void handleParts(CdbTransactionManager *trxManager, UndoLogPartsRepo *partsRepo, UndoLogsIdkey* key);

private:
	UnicodeString* srcSchema;
	UnicodeString* dstSchema;

	UnicodeString* srcTable;
	UnicodeString* dstTable;

};

} /* namespace codablecash */

#endif /* TRX_SESSION_DDL_ALTER_UNDOALTERRENAMETABLELOG_H_ */
