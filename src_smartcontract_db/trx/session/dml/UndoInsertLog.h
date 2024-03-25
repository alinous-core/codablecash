/*
 * UndoInsertLog.h
 *
 *  Created on: 2023/07/05
 *      Author: iizuka
 */

#ifndef TRX_SESSION_DML_UNDOINSERTLOG_H_
#define TRX_SESSION_DML_UNDOINSERTLOG_H_

#include "trx/session/dml/AbstractUndoDmlLog.h"

namespace codablecash {


class UndoInsertLog : public AbstractUndoDmlLog {
public:
	UndoInsertLog(const UndoInsertLog& inst);
	UndoInsertLog();
	virtual ~UndoInsertLog();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;
};

} /* namespace codablecash */

#endif /* TRX_SESSION_DML_UNDOINSERTLOG_H_ */
