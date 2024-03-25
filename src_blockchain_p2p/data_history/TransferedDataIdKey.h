/*
 * TransferedDataIdKey.h
 *
 *  Created on: 2023/09/18
 *      Author: iizuka
 */

#ifndef DATA_HISTORY_TRANSFEREDDATAIDKEY_H_
#define DATA_HISTORY_TRANSFEREDDATAIDKEY_H_

#include "btree/AbstractBtreeKey.h"

using namespace alinous;

namespace codablecash {

class TransferedDataId;

class TransferedDataIdKey : public AbstractBtreeKey {
public:
	TransferedDataIdKey();
	explicit TransferedDataIdKey(const TransferedDataId* dataId);
	virtual ~TransferedDataIdKey();

	virtual bool isInfinity() const { return false; }
	virtual bool isNull() const { return false; }

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	static TransferedDataIdKey* fromBinary(ByteBuffer* in);

	virtual int compareTo(const AbstractBtreeKey* key) const noexcept;
	virtual AbstractBtreeKey* clone() const noexcept;

private:
	TransferedDataId* dataId;
};

} /* namespace codablecash */

#endif /* DATA_HISTORY_TRANSFEREDDATAIDKEY_H_ */
