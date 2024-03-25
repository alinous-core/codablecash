/*
 * UndoLogsPartIdKey.h
 *
 *  Created on: 2023/07/10
 *      Author: iizuka
 */

#ifndef TRX_SESSION_REPO_UNDOLOGSPARTIDKEY_H_
#define TRX_SESSION_REPO_UNDOLOGSPARTIDKEY_H_

#include "btree/AbstractBtreeKey.h"

using namespace alinous;

namespace codablecash {

class CdbDatabaseSessionId;
class UndoLogsIdkey;
class CdbOid;

class UndoLogsPartIdKey : public AbstractBtreeKey {
public:
	UndoLogsPartIdKey(const UndoLogsPartIdKey& inst);
	UndoLogsPartIdKey();
	virtual ~UndoLogsPartIdKey();

	virtual bool isInfinity() const { return false; }
	virtual bool isNull() const { return false; }

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	static UndoLogsPartIdKey* fromBinary(ByteBuffer* in);

	virtual int compareTo(const AbstractBtreeKey* key) const noexcept;
	virtual AbstractBtreeKey* clone() const noexcept;

	void setLogIdKey(const UndoLogsIdkey* key) noexcept;
	void setPartSerial(uint64_t serial) noexcept;

	const CdbDatabaseSessionId* getSessionId() const noexcept;
	const CdbOid* getUndoLogSerial() const noexcept;

private:
	UndoLogsIdkey* logIdKey;
	CdbOid* partSerial;
};

} /* namespace codablecash */

#endif /* TRX_SESSION_REPO_UNDOLOGSPARTIDKEY_H_ */
