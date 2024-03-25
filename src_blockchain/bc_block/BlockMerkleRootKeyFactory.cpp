/*
 * BlockMerkleRootKeyFactory.cpp
 *
 *  Created on: 2023/03/02
 *      Author: iizuka
 */

#include "bc_block/BlockMerkleRootKeyFactory.h"
#include "bc_block/BlockMerkleRootKey.h"

namespace codablecash {

BlockMerkleRootKeyFactory::BlockMerkleRootKeyFactory() {

}

BlockMerkleRootKeyFactory::~BlockMerkleRootKeyFactory() {

}

AbstractBtreeKey* BlockMerkleRootKeyFactory::fromBinary(uint32_t keyType, ByteBuffer *in) const {
	if(keyType == BlockMerkleRootKeyFactory::MERKLE_ROOT_KEY){
		return BlockMerkleRootKey::fromBinary(in);
	}

	return BtreeKeyFactory::fromBinary(keyType, in);
}

BtreeKeyFactory* BlockMerkleRootKeyFactory::copy() const noexcept {
	return new BlockMerkleRootKeyFactory();
}

} /* namespace codablecash */
