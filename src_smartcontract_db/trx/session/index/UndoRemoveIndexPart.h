/*
 * UndoRemoveIndexPart.h
 *
 *  Created on: 2023/07/25
 *      Author: iizuka
 */

#ifndef TRX_SESSION_INDEX_UNDOREMOVEINDEXPART_H_
#define TRX_SESSION_INDEX_UNDOREMOVEINDEXPART_H_

#include "trx/session/base/AbstractUndoLogPart.h"

namespace codablecash {

class CdbTable;
class CdbTableIndex;

class UndoRemoveIndexPart : public AbstractUndoLogPart {
public:
	UndoRemoveIndexPart(const UndoRemoveIndexPart& inst);
	UndoRemoveIndexPart();
	virtual ~UndoRemoveIndexPart();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

	virtual void execute(CdbTransactionManager *trxManager, TableStore* store) const;

	void setTable(const CdbTable* tbl) noexcept;
	void setIndex(const CdbTableIndex* index) noexcept;

private:
	CdbTable* table;
	CdbTableIndex* removedindex;
};

} /* namespace codablecash */

#endif /* TRX_SESSION_INDEX_UNDOREMOVEINDEXPART_H_ */
