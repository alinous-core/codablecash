/*
 * TimeIndexKey.h
 *
 *  Created on: 2023/09/19
 *      Author: iizuka
 */

#ifndef DATA_HISTORY_TIMEINDEXKEY_H_
#define DATA_HISTORY_TIMEINDEXKEY_H_

#include "btree/AbstractBtreeKey.h"

using namespace alinous;

namespace codablecash {

class TransferedDataId;


class TimeIndexKey : public AbstractBtreeKey {
public:
	explicit TimeIndexKey(uint64_t value);
	TimeIndexKey();
	virtual ~TimeIndexKey();

	virtual bool isInfinity() const { return false; }
	virtual bool isNull() const { return false; }

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	static TimeIndexKey* fromBinary(ByteBuffer* in);

	virtual int compareTo(const AbstractBtreeKey* key) const noexcept;
	virtual AbstractBtreeKey* clone() const noexcept;

	const TransferedDataId* getDataId() const noexcept {
		return this->dataId;
	}
	void setDataId(const TransferedDataId* dataId) noexcept;

private:
	uint64_t tm;
	TransferedDataId* dataId;
};

} /* namespace codablecash */

#endif /* DATA_HISTORY_TIMEINDEXKEY_H_ */
