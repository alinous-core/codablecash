/*
 * UndoDeleteRecordPart.h
 *
 *  Created on: 2023/07/16
 *      Author: iizuka
 */

#ifndef TRX_SESSION_RECORD_UNDODELETERECORDPART_H_
#define TRX_SESSION_RECORD_UNDODELETERECORDPART_H_

#include "trx/session/base/AbstractUndoLogPart.h"

namespace codablecash {

class CdbRecord;

class UndoDeleteRecordPart: public AbstractUndoLogPart {
public:
	UndoDeleteRecordPart(const UndoDeleteRecordPart& inst);
	UndoDeleteRecordPart();
	virtual ~UndoDeleteRecordPart();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

	virtual void execute(CdbTransactionManager *trxManager, TableStore* store) const;

	void setRecord(const CdbRecord* record) noexcept;

private:
	CdbRecord* record;
};

} /* namespace codablecash */

#endif /* TRX_SESSION_RECORD_UNDODELETERECORDPART_H_ */
