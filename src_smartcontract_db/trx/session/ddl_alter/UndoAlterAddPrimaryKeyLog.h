/*
 * UndoAlterAddPrimaryKeyLog.h
 *
 *  Created on: 2023/08/04
 *      Author: iizuka
 */

#ifndef TRX_SESSION_DDL_ALTER_UNDOALTERADDPRIMARYKEYLOG_H_
#define TRX_SESSION_DDL_ALTER_UNDOALTERADDPRIMARYKEYLOG_H_

#include "trx/session/base/AbstractUndoLog.h"

namespace codablecash {

class CdbTable;

class UndoAlterAddPrimaryKeyLog : public AbstractUndoLog {
public:
	UndoAlterAddPrimaryKeyLog(const UndoAlterAddPrimaryKeyLog& inst);
	UndoAlterAddPrimaryKeyLog();
	virtual ~UndoAlterAddPrimaryKeyLog();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

	virtual void execute(CdbTransactionManager* trxManager, UndoLogPartsRepo* partsRepo, UndoLogsIdkey* key);

	void setTable(const CdbTable* tbl) noexcept;

private:
	void handleParts(CdbTransactionManager *trxManager, UndoLogPartsRepo *partsRepo, UndoLogsIdkey* key);

private:
	CdbTable* table;

};

} /* namespace codablecash */

#endif /* TRX_SESSION_DDL_ALTER_UNDOALTERADDPRIMARYKEYLOG_H_ */
