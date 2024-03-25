/*
 * TimeIndexKey.cpp
 *
 *  Created on: 2023/09/19
 *      Author: iizuka
 */

#include "data_history/TimeIndexKey.h"
#include "data_history/TimeIndexKeyFactory.h"
#include "data_history/TransferedDataId.h"

#include "osenv/funcs.h"

#include "base_io/ByteBuffer.h"

using alinous::Os;

namespace codablecash {

TimeIndexKey::TimeIndexKey(uint64_t value) {
	this->tm = value;
	this->dataId = nullptr;
}

TimeIndexKey::TimeIndexKey() {
	this->tm = Os::getTimestampLong();
	this->dataId = nullptr;
}

TimeIndexKey::~TimeIndexKey() {
	delete this->dataId;
}

int TimeIndexKey::binarySize() const {
	int size = sizeof(uint32_t);
	size += sizeof(this->tm);
	return size;
}

void TimeIndexKey::toBinary(ByteBuffer *out) const {
	out->putInt(TimeIndexKeyFactory::TIME_INDEX_KEY);
	out->putLong(this->tm);
}

TimeIndexKey* TimeIndexKey::fromBinary(ByteBuffer *in) {
	uint64_t value = in->getLong();
	return new TimeIndexKey(value);
}

int TimeIndexKey::compareTo(const AbstractBtreeKey *key) const noexcept {
	if(key->isInfinity()){
		return -1;
	}
	else if(key->isNull()){
		return 1;
	}

	const TimeIndexKey* ulkey = dynamic_cast<const TimeIndexKey*>(key);

	return this->tm > ulkey->tm ? 1 : (this->tm == ulkey->tm ? 0 : -1);
}

AbstractBtreeKey* TimeIndexKey::clone() const noexcept {
	return new TimeIndexKey(this->tm);
}

void TimeIndexKey::setDataId(const TransferedDataId *dataId) noexcept {
	delete this->dataId;
	this->dataId = dynamic_cast<TransferedDataId*>(dataId->copyData());
}

} /* namespace codablecash */
