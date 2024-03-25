/*
 * BlockMerkleRootKey.cpp
 *
 *  Created on: 2023/03/02
 *      Author: iizuka
 */

#include "bc_block/BlockMerkleRootKey.h"
#include "bc_block/BlockMerkleRoot.h"
#include "bc_block/BlockMerkleRootKeyFactory.h"

#include "bc_base/BinaryUtils.h"
#include "base/StackRelease.h"

#include "base_io/ByteBuffer.h"


namespace codablecash {

BlockMerkleRootKey::BlockMerkleRootKey(const BlockMerkleRoot* root) {
	this->root = root != nullptr ? dynamic_cast<BlockMerkleRoot*>(root->copyData()) : nullptr;
}

BlockMerkleRootKey::~BlockMerkleRootKey() {
	delete this->root;
}

int BlockMerkleRootKey::binarySize() const {
	BinaryUtils::checkNotNull(this->root);

	int size = sizeof(uint32_t);
	size += this->root->binarySize();

	return size;
}

void BlockMerkleRootKey::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->root);

	out->putInt(BlockMerkleRootKeyFactory::MERKLE_ROOT_KEY);

	this->root->toBinary(out);
}

BlockMerkleRootKey* BlockMerkleRootKey::fromBinary(ByteBuffer *in) {
	BlockMerkleRoot* root = BlockMerkleRoot::fromBinary(in); __STP(root);
	BinaryUtils::checkNotNull(root);

	return new BlockMerkleRootKey(root);
}

int BlockMerkleRootKey::compareTo(const AbstractBtreeKey *key) const noexcept {
	if(key->isInfinity()){
		return -1;
	}
	else if(key->isNull()){
		return 1;
	}

	const BlockMerkleRootKey* other = dynamic_cast<const BlockMerkleRootKey*>(key);
	return this->root->compareTo(other->root);
}

AbstractBtreeKey* BlockMerkleRootKey::clone() const noexcept {
	return new BlockMerkleRootKey(this->root);
}

} /* namespace codablecash */
