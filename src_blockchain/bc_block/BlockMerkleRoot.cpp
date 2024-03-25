/*
 * BlockMerkleRoot.cpp
 *
 *  Created on: 2023/03/02
 *      Author: iizuka
 */

#include "bc_block/BlockMerkleRoot.h"

namespace codablecash {

BlockMerkleRoot::BlockMerkleRoot(const char* binary, int length) : Abstract32BytesId(binary, length) {

}

BlockMerkleRoot::~BlockMerkleRoot() {

}

int BlockMerkleRoot::binarySize() const {
	return sizeof(uint8_t) * 32;
}

void BlockMerkleRoot::toBinary(ByteBuffer *out) const {
	out->put(this->id);
}

BlockMerkleRoot* BlockMerkleRoot::fromBinary(ByteBuffer *in) {
	uint8_t b[32];
	in->get(b, 32);

	return new BlockMerkleRoot((const char*)b, 32);
}

IBlockObject* BlockMerkleRoot::copyData() const noexcept {
	return new BlockMerkleRoot((const char*)this->id->array(), this->id->limit());
}

BlockMerkleRoot* BlockMerkleRoot::createZeroRoot() {
	char data[32];
	Mem::memset(data, 0, 32);
	return new BlockMerkleRoot(data, 32);
}

} /* namespace codablecash */
