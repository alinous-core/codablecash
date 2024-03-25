/*
 * TransactionIdData.cpp
 *
 *  Created on: 2023/02/27
 *      Author: iizuka
 */

#include "bc_base_trx_index/TransactionIdData.h"

#include "bc_trx/TransactionId.h"

#include "bc_base/BinaryUtils.h"

#include "base/StackRelease.h"

namespace codablecash {

TransactionIdData::TransactionIdData(const TransactionId* trxId) {
	this->trxId = dynamic_cast<TransactionId*>(trxId->copyData());
}

TransactionIdData::~TransactionIdData() {
	delete this->trxId;
}

int TransactionIdData::binarySize() const {
	BinaryUtils::checkNotNull(this->trxId);

	return this->trxId->binarySize();
}

void TransactionIdData::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->trxId);

	this->trxId->toBinary(out);
}

TransactionIdData* TransactionIdData::fromBinary(ByteBuffer *in) {
	TransactionId* trxId = TransactionId::fromBinary(in); __STP(trxId);

	return new TransactionIdData(trxId);
}

IBlockObject* TransactionIdData::copyData() const noexcept {
	return new TransactionIdData(this->trxId);
}

} /* namespace codablecash */
