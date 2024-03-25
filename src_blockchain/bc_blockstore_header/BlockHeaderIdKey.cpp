/*
 * BlockHeaderIdKey.cpp
 *
 *  Created on: 2023/05/01
 *      Author: iizuka
 */

#include "bc_blockstore_header/BlockHeaderIdKey.h"
#include "bc_blockstore_header/BlockHeaderIdKeyFactory.h"

#include "base_io/ByteBuffer.h"

#include "bc_block/BlockHeaderId.h"

#include "base/StackRelease.h"

namespace codablecash {

BlockHeaderIdKey::BlockHeaderIdKey(const BlockHeaderId *headerId) {
	this->headerId = dynamic_cast<BlockHeaderId*>(headerId->copyData());
}

BlockHeaderIdKey::~BlockHeaderIdKey() {
	delete this->headerId;
}

int BlockHeaderIdKey::binarySize() const {
	int size = sizeof(uint32_t);
	size += this->headerId->binarySize();

	return size;
}

void BlockHeaderIdKey::toBinary(ByteBuffer *out) const {
	out->putInt(BlockHeaderIdKeyFactory::BLOCK_HEADER_ID_KEY);
	this->headerId->toBinary(out);
}

BlockHeaderIdKey* BlockHeaderIdKey::fromBinary(ByteBuffer *in) {
	BlockHeaderId* headerId = BlockHeaderId::fromBinary(in); __STP(headerId);

	return new BlockHeaderIdKey(headerId);
}

int BlockHeaderIdKey::compareTo(const AbstractBtreeKey *key) const noexcept {
	if(key->isInfinity()){
		return -1;
	}
	else if(key->isNull()){
		return 1;
	}

	const BlockHeaderIdKey* other = dynamic_cast<const BlockHeaderIdKey*>(key);
	return this->headerId->compareTo(other->headerId);
}

AbstractBtreeKey* BlockHeaderIdKey::clone() const noexcept {
	return new BlockHeaderIdKey(this->headerId);
}

} /* namespace codablecash */
