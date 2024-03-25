/*
 * UndoAlterDropCplumnLog.h
 *
 *  Created on: 2023/07/31
 *      Author: iizuka
 */

#ifndef TRX_SESSION_DDL_ALTER_UNDOALTERDROPCOLUMNLOG_H_
#define TRX_SESSION_DDL_ALTER_UNDOALTERDROPCOLUMNLOG_H_

#include "trx/session/base/AbstractUndoLog.h"

namespace codablecash {

class CdbTable;
class CdbTableColumn;

class UndoAlterDropColumnLog : public AbstractUndoLog {
public:
	UndoAlterDropColumnLog(const UndoAlterDropColumnLog& inst);
	UndoAlterDropColumnLog();
	virtual ~UndoAlterDropColumnLog();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

	virtual void execute(CdbTransactionManager* trxManager, UndoLogPartsRepo* partsRepo, UndoLogsIdkey* key);

	void setTable(const CdbTable* table) noexcept;
	void setRemoveColumn(const CdbTableColumn* col) noexcept;

private:
	void handleParts(CdbTransactionManager *trxManager, UndoLogPartsRepo *partsRepo, UndoLogsIdkey* key);

private:
	CdbTable* table;
	CdbTableColumn* removedColumn;
};

} /* namespace codablecash */

#endif /* TRX_SESSION_DDL_ALTER_UNDOALTERDROPCOLUMNLOG_H_ */
