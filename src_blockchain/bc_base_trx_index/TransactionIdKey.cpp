/*
 * TransactionIdKey.cpp
 *
 *  Created on: 2023/02/23
 *      Author: iizuka
 */

#include "bc_base_trx_index/TransactionIdKey.h"
#include "bc_base_trx_index/TransactionIdKeyFactory.h"

#include "bc_trx/TransactionId.h"

#include "bc_base/BinaryUtils.h"

#include "base/StackRelease.h"


namespace codablecash {

TransactionIdKey::TransactionIdKey() {
	this->trxId = nullptr;
}

TransactionIdKey::TransactionIdKey(const TransactionId* trxId) {
	this->trxId = dynamic_cast<TransactionId*>(trxId->copyData());
}

TransactionIdKey::~TransactionIdKey() {
	delete this->trxId;
}

int TransactionIdKey::binarySize() const {
	BinaryUtils::checkNotNull(this->trxId);

	int size = sizeof(uint32_t);
	size += this->trxId->binarySize();

	return size;
}

void TransactionIdKey::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->trxId);

	out->putInt(TransactionIdKeyFactory::TRANSACTION_ID_KEY);

	this->trxId->toBinary(out);
}

TransactionIdKey* TransactionIdKey::fromBinary(ByteBuffer *in) {
	TransactionId* trxId = TransactionId::fromBinary(in); __STP(trxId);
	BinaryUtils::checkNotNull(trxId);

	return new TransactionIdKey(trxId);
}

int TransactionIdKey::compareTo(const AbstractBtreeKey *key) const noexcept {
	if(key->isInfinity()){
		return -1;
	}
	else if(key->isNull()){
		return 1;
	}

	const TransactionIdKey* other = dynamic_cast<const TransactionIdKey*>(key);
	assert(other != nullptr);

	return this->trxId->compareTo(other->trxId);
}

AbstractBtreeKey* TransactionIdKey::clone() const noexcept {
	return new TransactionIdKey(this->trxId);
}

} /* namespace codablecash */
