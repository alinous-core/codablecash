/*
 * SystemTimestampKeyFactory.cpp
 *
 *  Created on: Jun 7, 2026
 *      Author: iizuka
 */

#include "base_timestamp/SystemTimestampKeyFactory.h"
#include "base_timestamp/SystemTimestampKey.h"

#include "base_io/ByteBuffer.h"


namespace codablecash {

SystemTimestampKeyFactory::SystemTimestampKeyFactory() {

}

SystemTimestampKeyFactory::~SystemTimestampKeyFactory() {
}

AbstractBtreeKey* SystemTimestampKeyFactory::fromBinary(uint32_t keyType, ByteBuffer *in) const {
	if(keyType == SystemTimestampKeyFactory::SYSTEM_TIMESTAMP_KEY){
		AbstractBtreeKey* key = SystemTimestampKey::fromBinary(in);
		return key;
	}

	return BtreeKeyFactory::fromBinary(keyType, in);
}

BtreeKeyFactory* SystemTimestampKeyFactory::copy() const noexcept {
	return new SystemTimestampKeyFactory();
}

} /* namespace codablecash */
