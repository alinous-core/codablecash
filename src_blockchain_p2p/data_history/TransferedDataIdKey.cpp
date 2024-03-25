/*
 * TransferedDataIdKey.cpp
 *
 *  Created on: 2023/09/18
 *      Author: iizuka
 */

#include "data_history/TransferedDataIdKey.h"
#include "data_history/TransferedDataId.h"
#include "data_history/TransferedDataIdKeyFactory.h"

#include "bc_base/BinaryUtils.h"

#include "base/StackRelease.h"

namespace codablecash {

TransferedDataIdKey::TransferedDataIdKey() {
	this->dataId = nullptr;
}

TransferedDataIdKey::TransferedDataIdKey(const TransferedDataId *dataId) {
	this->dataId = dataId != nullptr ? dynamic_cast<TransferedDataId*>(dataId->copyData()) : nullptr;
}

TransferedDataIdKey::~TransferedDataIdKey() {
	delete this->dataId;
}

int TransferedDataIdKey::binarySize() const {
	BinaryUtils::checkNotNull(this->dataId);

	int size = sizeof(uint32_t);
	size += this->dataId->binarySize();

	return size;
}

void TransferedDataIdKey::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->dataId);

	out->putInt(TransferedDataIdKeyFactory::TRANSFERED_DATA_ID_KEY);

	this->dataId->toBinary(out);
}

TransferedDataIdKey* TransferedDataIdKey::fromBinary(ByteBuffer *in) {
	TransferedDataId* trxId = TransferedDataId::fromBinary(in); __STP(trxId);
	BinaryUtils::checkNotNull(trxId);

	return new TransferedDataIdKey(trxId);
}

int TransferedDataIdKey::compareTo(const AbstractBtreeKey *key) const noexcept {
	if(key->isInfinity()){
		return -1;
	}
	else if(key->isNull()){
		return 1;
	}

	const TransferedDataIdKey* other = dynamic_cast<const TransferedDataIdKey*>(key);
	assert(other != nullptr);

	return this->dataId->compareTo(other->dataId);
}

AbstractBtreeKey* TransferedDataIdKey::clone() const noexcept {
	return new TransferedDataIdKey(this->dataId);
}

} /* namespace codablecash */
