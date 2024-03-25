/*
 * AbstractUndoDmlLog.h
 *
 *  Created on: 2023/08/06
 *      Author: iizuka
 */

#ifndef TRX_SESSION_DML_ABSTRACTUNDODMLLOG_H_
#define TRX_SESSION_DML_ABSTRACTUNDODMLLOG_H_

#include "trx/session/base/AbstractUndoLog.h"

namespace codablecash {

class CdbTableIdentifier;

class AbstractUndoDmlLog : public AbstractUndoLog {
public:
	AbstractUndoDmlLog(const AbstractUndoDmlLog& inst);
	explicit AbstractUndoDmlLog(uint8_t type);
	virtual ~AbstractUndoDmlLog();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual void execute(CdbTransactionManager* trxManager, UndoLogPartsRepo* partsRepo, UndoLogsIdkey* key);

	void setTableId(const CdbTableIdentifier* table) noexcept;

protected:
	void handleParts(CdbTransactionManager *trxManager, UndoLogPartsRepo *partsRepo, UndoLogsIdkey* key);

protected:
	CdbTableIdentifier* table;
};

} /* namespace codablecash */

#endif /* TRX_SESSION_DML_ABSTRACTUNDODMLLOG_H_ */
