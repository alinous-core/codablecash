/*
 * UndoIndexPrimaryPart.h
 *
 *  Created on: 2023/08/04
 *      Author: iizuka
 */

#ifndef TRX_SESSION_INDEX_UNDOINDEXPRIMARYPART_H_
#define TRX_SESSION_INDEX_UNDOINDEXPRIMARYPART_H_

#include <cstdint>

#include "trx/session/base/AbstractUndoLogPart.h"

namespace codablecash {

class CdbTable;

class UndoIndexPrimaryPart : public AbstractUndoLogPart {
public:
	UndoIndexPrimaryPart(const UndoIndexPrimaryPart& inst);
	UndoIndexPrimaryPart();
	virtual ~UndoIndexPrimaryPart();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

	virtual void execute(CdbTransactionManager *trxManager, TableStore* store) const;

	void setTable(const CdbTable* tbl) noexcept;
	void setIndexOid(uint64_t oid) noexcept;
	void setLastPrimary(bool bl) noexcept;

private:
	CdbTable* table;
	uint64_t indexOid;
	bool lastPrimary;
};

} /* namespace codablecash */

#endif /* TRX_SESSION_INDEX_UNDOINDEXPRIMARYPART_H_ */
