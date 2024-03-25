/*
 * UnodCreateTableLog.h
 *
 *  Created on: 2023/07/05
 *      Author: iizuka
 */

#ifndef TRX_SESSION_DDL_UNDOCREATETABLELOG_H_
#define TRX_SESSION_DDL_UNDOCREATETABLELOG_H_

#include "trx/session/base/AbstractUndoLog.h"

namespace codablecash {

class CdbOid;

class UndoCreateTableLog : public AbstractUndoLog {
public:
	UndoCreateTableLog(const UndoCreateTableLog& inst);
	UndoCreateTableLog();
	virtual ~UndoCreateTableLog();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

	void setTableOid(const CdbOid* oid) noexcept;

	virtual void execute(CdbTransactionManager* trxManager, UndoLogPartsRepo* partsRepo, UndoLogsIdkey* key);

private:
	CdbOid* tableOid;
};

} /* namespace codablecash */

#endif /* TRX_SESSION_DDL_UNDOCREATETABLELOG_H_ */
