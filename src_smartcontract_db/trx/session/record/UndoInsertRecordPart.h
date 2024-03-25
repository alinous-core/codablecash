/*
 * UndoInsertRecordPart.h
 *
 *  Created on: 2023/07/27
 *      Author: iizuka
 */

#ifndef TRX_SESSION_RECORD_UNDOINSERTRECORDPART_H_
#define TRX_SESSION_RECORD_UNDOINSERTRECORDPART_H_

#include "trx/session/base/AbstractUndoLogPart.h"

namespace codablecash {

class UndoInsertRecordPart : public AbstractUndoLogPart {
public:
	UndoInsertRecordPart(const UndoInsertRecordPart& inst);
	UndoInsertRecordPart();
	virtual ~UndoInsertRecordPart();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

	virtual void execute(CdbTransactionManager *trxManager, TableStore* store) const;

	void setRecordOid(uint64_t oid) noexcept;

private:
	uint64_t recordOid;
};

} /* namespace codablecash */

#endif /* TRX_SESSION_RECORD_UNDOINSERTRECORDPART_H_ */
