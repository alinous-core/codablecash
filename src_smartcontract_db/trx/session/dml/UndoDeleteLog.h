/*
 * UndoDeleteLog.h
 *
 *  Created on: 2023/08/06
 *      Author: iizuka
 */

#ifndef TRX_SESSION_DML_UNDODELETELOG_H_
#define TRX_SESSION_DML_UNDODELETELOG_H_

#include "trx/session/dml/AbstractUndoDmlLog.h"

namespace codablecash {

class UndoDeleteLog : public AbstractUndoDmlLog {
public:
	UndoDeleteLog(const UndoDeleteLog& inst);
	UndoDeleteLog();
	virtual ~UndoDeleteLog();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;
};

} /* namespace codablecash */

#endif /* TRX_SESSION_DML_UNDODELETELOG_H_ */
