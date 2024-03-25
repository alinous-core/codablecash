/*
 * UndoLogsIdkey.h
 *
 *  Created on: 2023/07/10
 *      Author: iizuka
 */

#ifndef TRX_SESSION_REPO_UNDOLOGSIDKEY_H_
#define TRX_SESSION_REPO_UNDOLOGSIDKEY_H_

#include "btree/AbstractBtreeKey.h"

using namespace alinous;

namespace codablecash {

class CdbDatabaseSessionId;
class CdbOid;

class UndoLogsIdkey : public AbstractBtreeKey {
public:
	UndoLogsIdkey(const UndoLogsIdkey& inst);
	UndoLogsIdkey();
	virtual ~UndoLogsIdkey();

	virtual bool isInfinity() const { return false; }
	virtual bool isNull() const { return false; }

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	static UndoLogsIdkey* fromBinary(ByteBuffer* in);

	virtual int compareTo(const AbstractBtreeKey* key) const noexcept;
	virtual AbstractBtreeKey* clone() const noexcept;

	void setSessionId(const CdbDatabaseSessionId* id) noexcept;
	void setSerial(uint64_t serial) noexcept;

	const CdbDatabaseSessionId* getSessionId() const noexcept {
		return this->sessionId;
	}
	const CdbOid* getUndoLogSerial() const noexcept {
		return this->undoLogSerial;
	}

private:
	CdbDatabaseSessionId* sessionId;
	CdbOid* undoLogSerial;
};

} /* namespace codablecash */

#endif /* TRX_SESSION_REPO_UNDOLOGSIDKEY_H_ */
