/*
 * BlockHeaderHeightIndexKeyFactory.cpp
 *
 *  Created on: 2023/02/16
 *      Author: iizuka
 */

#include "bc_blockstore_header/BlockHeaderHeightIndexKeyFactory.h"
#include "bc_blockstore_header/BlockHeaderHeightIndexKey.h"

namespace codablecash {

BlockHeaderHeightIndexKeyFactory::BlockHeaderHeightIndexKeyFactory() {

}

BlockHeaderHeightIndexKeyFactory::~BlockHeaderHeightIndexKeyFactory() {

}

AbstractBtreeKey* BlockHeaderHeightIndexKeyFactory::fromBinary(uint32_t keyType, ByteBuffer *in) const {
	if(keyType == BlockHeaderHeightIndexKeyFactory::HEIGHT_INDEX_KEY){
		return BlockHeaderHeightIndexKey::fromBinary(in);
	}

	return BtreeKeyFactory::fromBinary(keyType, in);
}

BtreeKeyFactory* BlockHeaderHeightIndexKeyFactory::copy() const noexcept {
	return new BlockHeaderHeightIndexKeyFactory();
}

} /* namespace codablecash */
