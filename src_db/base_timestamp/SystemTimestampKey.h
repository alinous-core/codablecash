/*
 * SystemTimestampKey.h
 *
 *  Created on: Jun 6, 2026
 *      Author: iizuka
 */

#ifndef BASE_TIMESTAMP_SYSTEMTIMESTAMPKEY_H_
#define BASE_TIMESTAMP_SYSTEMTIMESTAMPKEY_H_

#include "btree/AbstractBtreeKey.h"

namespace alinous {
class SystemTimestamp;
}
using namespace alinous;

namespace codablecash {

class SystemTimestampKey : public AbstractBtreeKey {
public:
	explicit SystemTimestampKey(const SystemTimestamp* tm);
	virtual ~SystemTimestampKey();

	virtual bool isInfinity() const { return false; }
	virtual bool isNull() const { return false; }

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	static SystemTimestampKey* fromBinary(ByteBuffer* in);

	virtual int compareTo(const AbstractBtreeKey* key) const noexcept;
	virtual AbstractBtreeKey* clone() const noexcept;

	void setRemoveKey(const IBlockObject* rkey);
	const IBlockObject* getRemoveKey() const noexcept {
		return this->removeKey;
	}

private:
	SystemTimestamp* tm;
	IBlockObject* removeKey;
};

} /* namespace codablecash */

#endif /* BASE_TIMESTAMP_SYSTEMTIMESTAMPKEY_H_ */
