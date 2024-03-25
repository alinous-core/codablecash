/*
 * UndoUpdateRecordColumnPart.h
 *
 *  Created on: 2023/07/25
 *      Author: iizuka
 */

#ifndef TRX_SESSION_RECORD_UNDOMODIFYRECORDCOLUMNSPART_H_
#define TRX_SESSION_RECORD_UNDOMODIFYRECORDCOLUMNSPART_H_

#include "trx/session/base/AbstractUndoLogPart.h"

namespace codablecash {

class UpdatedColumn;
class AbstractCdbValue;
class CdbOid;

class UndoModifyRecordColumnsPart : public AbstractUndoLogPart {
public:
	UndoModifyRecordColumnsPart(const UndoModifyRecordColumnsPart& inst);
	UndoModifyRecordColumnsPart();
	virtual ~UndoModifyRecordColumnsPart();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

	virtual void execute(CdbTransactionManager *trxManager, TableStore* store) const;

	void setRecordOid(const CdbOid* oid) noexcept;
	void addUpdatedColumn(int pos, const AbstractCdbValue *lastValue)noexcept;

private:
	CdbOid* recordOid;
	ArrayList<UpdatedColumn>* list;

};

} /* namespace codablecash */

#endif /* TRX_SESSION_RECORD_UNDOMODIFYRECORDCOLUMNSPART_H_ */
