/*
 * UndoCreateSchemaPart.h
 *
 *  Created on: 2023/08/04
 *      Author: iizuka
 */

#ifndef TRX_SESSION_RECORD_UNDOCREATESCHEMAPART_H_
#define TRX_SESSION_RECORD_UNDOCREATESCHEMAPART_H_

#include "trx/session/base/AbstractUndoLogPart.h"

namespace codablecash {

class UndoCreateSchemaPart : public AbstractUndoLogPart {
public:
	UndoCreateSchemaPart(const UndoCreateSchemaPart& inst);
	UndoCreateSchemaPart();
	virtual ~UndoCreateSchemaPart();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

	virtual void execute(CdbTransactionManager *trxManager, TableStore* store) const;

	void setSchemaName(const UnicodeString* name) noexcept;

private:
	UnicodeString* schemaName;

};

} /* namespace codablecash */

#endif /* TRX_SESSION_RECORD_UNDOCREATESCHEMAPART_H_ */
