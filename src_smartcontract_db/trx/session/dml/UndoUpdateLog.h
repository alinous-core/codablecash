/*
 * UndoUpdateLog.h
 *
 *  Created on: 2023/08/06
 *      Author: iizuka
 */

#ifndef TRX_SESSION_DML_UNDOUPDATELOG_H_
#define TRX_SESSION_DML_UNDOUPDATELOG_H_

#include "trx/session/dml/AbstractUndoDmlLog.h"

namespace codablecash {

class UndoUpdateLog : public AbstractUndoDmlLog {
public:
	UndoUpdateLog(const UndoUpdateLog& inst);
	UndoUpdateLog();
	virtual ~UndoUpdateLog();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;
};

} /* namespace codablecash */

#endif /* TRX_SESSION_DML_UNDOUPDATELOG_H_ */
