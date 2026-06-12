/*
 * SystemTimestampKey.cpp
 *
 *  Created on: Jun 6, 2026
 *      Author: iizuka
 */

#include "base_timestamp/SystemTimestampKey.h"
#include "base_timestamp/SystemTimestamp.h"
#include "base_timestamp/SystemTimestampKeyFactory.h"

#include "bc_base/BinaryUtils.h"

#include "base/StackRelease.h"

namespace codablecash {

SystemTimestampKey::SystemTimestampKey(const SystemTimestamp* tm) {
	this->tm = new SystemTimestamp(*tm);
	this->removeKey = nullptr;
}

SystemTimestampKey::~SystemTimestampKey() {
	delete this->tm;
}

int SystemTimestampKey::binarySize() const {
	BinaryUtils::checkNotNull(this->tm);

	int size = sizeof(uint32_t);
	size += this->tm->binarySize();

	return size;
}

void SystemTimestampKey::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->tm);

	out->putInt(SystemTimestampKeyFactory::SYSTEM_TIMESTAMP_KEY);

	this->tm->toBinary(out);
}

SystemTimestampKey* SystemTimestampKey::fromBinary(ByteBuffer *in) {
	SystemTimestamp* tm = SystemTimestamp::fromBinary(in); __STP(tm);
	BinaryUtils::checkNotNull(tm);

	return new SystemTimestampKey(tm);
}

int SystemTimestampKey::compareTo(const AbstractBtreeKey *key) const noexcept {
	if(key->isInfinity()){
		return -1;
	}
	else if(key->isNull()){
		return 1;
	}

	const SystemTimestampKey* other = dynamic_cast<const SystemTimestampKey*>(key);
	assert(other != nullptr);

	return this->tm->compareTo(other->tm);
}

AbstractBtreeKey* SystemTimestampKey::clone() const noexcept {
	return new SystemTimestampKey(this->tm);
}

void SystemTimestampKey::setRemoveKey(const IBlockObject *rkey) {
	delete this->removeKey, this->removeKey = nullptr;
	this->removeKey = rkey->copyData();
}

} /* namespace codablecash */
