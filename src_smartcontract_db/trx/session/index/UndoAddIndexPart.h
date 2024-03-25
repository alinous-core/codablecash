/*
 * UndoAddIndexPart.h
 *
 *  Created on: 2023/07/25
 *      Author: iizuka
 */

#ifndef TRX_SESSION_INDEX_UNDOADDINDEXPART_H_
#define TRX_SESSION_INDEX_UNDOADDINDEXPART_H_

#include "trx/session/base/AbstractUndoLogPart.h"

namespace codablecash {

class CdbTable;

class UndoAddIndexPart : public AbstractUndoLogPart {
public:
	UndoAddIndexPart(const UndoAddIndexPart& inst);
	UndoAddIndexPart();
	virtual ~UndoAddIndexPart();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

	virtual void execute(CdbTransactionManager *trxManager, TableStore* store) const;

	void setTable(const CdbTable* tbl) noexcept;
	void setIndexOid(uint64_t oid) noexcept;

private:
	CdbTable* table;
	uint64_t indexOid;
};

} /* namespace codablecash */

#endif /* TRX_SESSION_INDEX_UNDOADDINDEXPART_H_ */
