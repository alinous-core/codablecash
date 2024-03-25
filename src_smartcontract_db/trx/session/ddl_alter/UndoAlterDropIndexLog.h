/*
 * UndoAlterDropIndexLog.h
 *
 *  Created on: 2023/07/24
 *      Author: iizuka
 */

#ifndef TRX_SESSION_DDL_ALTER_UNDOALTERDROPINDEXLOG_H_
#define TRX_SESSION_DDL_ALTER_UNDOALTERDROPINDEXLOG_H_

#include "trx/session/base/AbstractUndoLog.h"

namespace codablecash {

class CdbTableIndex;
class CdbTable;

class UndoAlterDropIndexLog : public AbstractUndoLog {
public:
	UndoAlterDropIndexLog(const UndoAlterDropIndexLog& inst);
	UndoAlterDropIndexLog();
	virtual ~UndoAlterDropIndexLog();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

	virtual void execute(CdbTransactionManager* trxManager, UndoLogPartsRepo* partsRepo, UndoLogsIdkey* key);

	void setTable(const CdbTable* table) noexcept;
	void setTableIndex(const CdbTableIndex* removalIndex) noexcept;

private:
	CdbTable* table;
	CdbTableIndex* removalIndex;
};

} /* namespace codablecash */

#endif /* TRX_SESSION_DDL_ALTER_UNDOALTERDROPINDEXLOG_H_ */
