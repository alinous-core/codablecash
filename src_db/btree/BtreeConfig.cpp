/*
 * BtreeConfig.cpp
 *
 *  Created on: 2018/12/22
 *      Author: iizuka
 */

#include "btree/BtreeConfig.h"
#include "base_io/ByteBuffer.h"

namespace alinous {

BtreeConfig::BtreeConfig(const BtreeConfig* inst) {
	this->defaultSize = inst->defaultSize;
	this->blockSize = inst->blockSize;
	this->nodeNumber = inst->nodeNumber;
}

BtreeConfig::BtreeConfig() {
	this->defaultSize = 1024;
	this->blockSize = 64;
	this->nodeNumber = 8;
}

BtreeConfig::~BtreeConfig() {
}

int BtreeConfig::binarySize() const {
	return sizeof(this->defaultSize) + sizeof(this->blockSize) + sizeof(this->nodeNumber);
}

void BtreeConfig::toBinary(ByteBuffer* out) const {
	out->putLong(this->defaultSize);
	out->putLong(this->blockSize);
	out->putLong(this->nodeNumber);
}

BtreeConfig* BtreeConfig::fromBinary(ByteBuffer* in) {
	BtreeConfig* inst = new BtreeConfig();

	inst->defaultSize = in->getLong();
	inst->blockSize = in->getLong();
	inst->nodeNumber = in->getLong();

	return inst;
}

IBlockObject* BtreeConfig::copyData() const noexcept {
	return new BtreeConfig(*this);
}

} /* namespace alinous */
