/*
 * UndoIndexUniquePart.h
 *
 *  Created on: 2023/07/24
 *      Author: iizuka
 */

#ifndef TRX_SESSION_INDEX_UNDOINDEXUNIQUEPART_H_
#define TRX_SESSION_INDEX_UNDOINDEXUNIQUEPART_H_

#include "trx/session/base/AbstractUndoLogPart.h"

namespace codablecash {

class CdbTable;

class UndoIndexUniquePart : public AbstractUndoLogPart {
public:
	UndoIndexUniquePart(const UndoIndexUniquePart& inst);
	UndoIndexUniquePart();
	virtual ~UndoIndexUniquePart();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

	virtual void execute(CdbTransactionManager *trxManager, TableStore* store) const;

	void setTable(const CdbTable* tbl) noexcept;
	void setIndexOid(uint64_t oid) noexcept;
	void setLastUnique(bool bl) noexcept;

private:
	CdbTable* table;
	uint64_t indexOid;
	bool lastUnique;
};

} /* namespace codablecash */

#endif /* TRX_SESSION_INDEX_UNDOINDEXUNIQUEPART_H_ */
