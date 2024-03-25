/*
 * BlockHeaderIdKeyFactory.cpp
 *
 *  Created on: 2023/05/01
 *      Author: iizuka
 */

#include "bc_blockstore_header/BlockHeaderIdKeyFactory.h"
#include "bc_blockstore_header/BlockHeaderIdKey.h"

namespace codablecash {

BlockHeaderIdKeyFactory::BlockHeaderIdKeyFactory() {

}

BlockHeaderIdKeyFactory::~BlockHeaderIdKeyFactory() {

}

AbstractBtreeKey* BlockHeaderIdKeyFactory::fromBinary(uint32_t keyType, ByteBuffer *in) const {
	if(keyType == BlockHeaderIdKeyFactory::BLOCK_HEADER_ID_KEY){
		return BlockHeaderIdKey::fromBinary(in);
	}

	return BtreeKeyFactory::fromBinary(keyType, in);
}

BtreeKeyFactory* BlockHeaderIdKeyFactory::copy() const noexcept {
	return new BlockHeaderIdKeyFactory();
}

} /* namespace codablecash */
