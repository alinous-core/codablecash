/*
 * InstanceIdIndexKey.cpp
 *
 *  Created on: Nov 28, 2025
 *      Author: iizuka
 */

#include "smartcontract_executor_index/InstanceIdIndexKey.h"
#include "smartcontract_executor_index/InstanceIdIndexKeyFactory.h"

#include "transaction/SmartcontractInstanceAddress.h"

#include "btree/AbstractBtreeKey.h"

#include "btreekey/ULongKey.h"

#include "bc_base/BinaryUtils.h"
#include "bc_base/AbstractAddress.h"

#include "base_io/ByteBuffer.h"

#include "base/StackRelease.h"

namespace codablecash {

InstanceIdIndexKey::InstanceIdIndexKey() {
	this->instanceAddress = nullptr;
	this->height = 0;
}

InstanceIdIndexKey::InstanceIdIndexKey(const SmartcontractInstanceAddress *instanceAddress, uint64_t height) {
	this->instanceAddress = instanceAddress != nullptr ? dynamic_cast<SmartcontractInstanceAddress*>(instanceAddress->copyData()) : nullptr;
	this->height = height;
}

InstanceIdIndexKey::~InstanceIdIndexKey() {
	delete this->instanceAddress;
}

int InstanceIdIndexKey::binarySize() const {
	BinaryUtils::checkNotNull(this->instanceAddress);

	int size = sizeof(uint32_t);
	size += this->instanceAddress->binarySize();
	size += sizeof(uint64_t);

	return size;
}

void InstanceIdIndexKey::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->instanceAddress);

	out->putInt(InstanceIdIndexKeyFactory::INSTANCE_INDEX_KEY);
	this->instanceAddress->toBinary(out);
	out->putLong(this->height);
}

InstanceIdIndexKey* InstanceIdIndexKey::fromBinary(ByteBuffer *in) {
	AbstractAddress* address = SmartcontractInstanceAddress::createFromBinary(in); __STP(address);
	SmartcontractInstanceAddress* contractAddress = dynamic_cast<SmartcontractInstanceAddress*>(address);
	BinaryUtils::checkNotNull(contractAddress);

	uint64_t height = in->getLong();

	return new InstanceIdIndexKey(contractAddress, height);
}

int InstanceIdIndexKey::compareTo(const AbstractBtreeKey *key) const noexcept {
	if(key->isInfinity()){
		return -1;
	}
	else if(key->isNull()){
		return 1;
	}

	const InstanceIdIndexKey* other = dynamic_cast<const InstanceIdIndexKey*>(key);
	assert(other != nullptr);

	return this->instanceAddress->compareTo(other->instanceAddress);
}

AbstractBtreeKey* InstanceIdIndexKey::clone() const noexcept {
	return new InstanceIdIndexKey(this->instanceAddress, this->height);
}

void InstanceIdIndexKey::setHeight(uint64_t height) {
	this->height = height;
}

} /* namespace codablecash */
