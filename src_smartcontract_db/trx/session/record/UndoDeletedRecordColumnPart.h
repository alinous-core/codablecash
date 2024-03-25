/*
 * UndoDeletedRecordColumn.h
 *
 *  Created on: 2023/08/01
 *      Author: iizuka
 */

#ifndef TRX_SESSION_RECORD_UNDODELETEDRECORDCOLUMNPART_H_
#define TRX_SESSION_RECORD_UNDODELETEDRECORDCOLUMNPART_H_

#include "trx/session/base/AbstractUndoLogPart.h"

namespace codablecash {

class AbstractCdbValue;

class UndoDeletedRecordColumnPart : public AbstractUndoLogPart {
public:
	UndoDeletedRecordColumnPart(const UndoDeletedRecordColumnPart& inst);
	UndoDeletedRecordColumnPart();
	virtual ~UndoDeletedRecordColumnPart();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

	virtual void execute(CdbTransactionManager *trxManager, TableStore* store) const;

	void setRecordOid(uint64_t oid) noexcept;
	void setPosition(int position) noexcept;
	void setLastValue(const AbstractCdbValue* value) noexcept;

private:
	uint64_t recordOid;
	uint16_t pos;
	AbstractCdbValue *lastValue;
};

} /* namespace codablecash */

#endif /* TRX_SESSION_RECORD_UNDODELETEDRECORDCOLUMNPART_H_ */
