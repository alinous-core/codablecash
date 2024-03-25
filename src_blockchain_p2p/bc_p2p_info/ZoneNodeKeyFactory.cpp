/*
 * ZoneNodeKeyFactory.cpp
 *
 *  Created on: 2023/09/07
 *      Author: iizuka
 */

#include "bc_p2p_info/ZoneNodeKeyFactory.h"
#include "bc_p2p_info/ZoneNodeKey.h"

namespace codablecash {

ZoneNodeKeyFactory::ZoneNodeKeyFactory() {

}

ZoneNodeKeyFactory::~ZoneNodeKeyFactory() {

}

AbstractBtreeKey* ZoneNodeKeyFactory::fromBinary(uint32_t keyType, ByteBuffer *in) const {
	if(keyType == ZONE_NODE_KEY){
		return ZoneNodeKey::fromBinary(in);
	}

	return BtreeKeyFactory::fromBinary(keyType, in);
}

BtreeKeyFactory* ZoneNodeKeyFactory::copy() const noexcept {
	return new ZoneNodeKeyFactory();
}

} /* namespace codablecash */
