/*
 * TimeIndexKeyFactory.cpp
 *
 *  Created on: 2023/09/19
 *      Author: iizuka
 */

#include "data_history/TimeIndexKeyFactory.h"
#include "data_history/TimeIndexKey.h"

namespace codablecash {

TimeIndexKeyFactory::TimeIndexKeyFactory() {

}

TimeIndexKeyFactory::~TimeIndexKeyFactory() {

}

AbstractBtreeKey* TimeIndexKeyFactory::fromBinary(uint32_t keyType, ByteBuffer *in) const {
	if(keyType == TIME_INDEX_KEY){
		AbstractBtreeKey* key = TimeIndexKey::fromBinary(in);
		return key;
	}

	return BtreeKeyFactory::fromBinary(keyType, in);
}

BtreeKeyFactory* TimeIndexKeyFactory::copy() const noexcept {
	return new TimeIndexKeyFactory();
}

} /* namespace codablecash */
