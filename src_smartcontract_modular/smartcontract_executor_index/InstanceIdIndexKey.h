/*
 * InstanceIdIndexKey.h
 *
 *  Created on: Nov 28, 2025
 *      Author: iizuka
 */

#ifndef SMARTCONTRACT_EXECUTOR_INDEX_INSTANCEIDINDEXKEY_H_
#define SMARTCONTRACT_EXECUTOR_INDEX_INSTANCEIDINDEXKEY_H_

#include "btree/AbstractBtreeKey.h"

namespace alinous {
class AbstractBtreeKey;
}
using namespace alinous;

namespace codablecash {

class SmartcontractInstanceAddress;

class InstanceIdIndexKey : public AbstractBtreeKey {
public:
	InstanceIdIndexKey();
	InstanceIdIndexKey(const SmartcontractInstanceAddress* instanceAddress, uint64_t height);
	virtual ~InstanceIdIndexKey();

	virtual bool isInfinity() const { return false; }
	virtual bool isNull() const { return false; }

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	static InstanceIdIndexKey* fromBinary(ByteBuffer* in);

	virtual int compareTo(const AbstractBtreeKey* key) const noexcept;
	virtual AbstractBtreeKey* clone() const noexcept;

	void setHeight(uint64_t height);


private:
	SmartcontractInstanceAddress* instanceAddress;
	uint64_t height;
};

} /* namespace codablecash */

#endif /* SMARTCONTRACT_EXECUTOR_INDEX_INSTANCEIDINDEXKEY_H_ */
