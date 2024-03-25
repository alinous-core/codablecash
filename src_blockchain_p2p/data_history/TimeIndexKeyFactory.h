/*
 * TimeIndexKeyFactory.h
 *
 *  Created on: 2023/09/19
 *      Author: iizuka
 */

#ifndef DATA_HISTORY_TIMEINDEXKEYFACTORY_H_
#define DATA_HISTORY_TIMEINDEXKEYFACTORY_H_

#include "btreekey/BtreeKeyFactory.h"

using namespace alinous;

namespace codablecash {

class TimeIndexKeyFactory : public BtreeKeyFactory {
public:
	static const constexpr uint32_t TIME_INDEX_KEY{0x03};

	TimeIndexKeyFactory();
	virtual ~TimeIndexKeyFactory();

	virtual AbstractBtreeKey* fromBinary(uint32_t keyType, ByteBuffer* in) const;
	virtual BtreeKeyFactory* copy() const noexcept;
};

} /* namespace codablecash */

#endif /* DATA_HISTORY_TIMEINDEXKEYFACTORY_H_ */
