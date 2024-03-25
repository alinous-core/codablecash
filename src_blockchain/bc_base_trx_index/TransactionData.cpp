/*
 * TransactionData.cpp
 *
 *  Created on: 2023/02/23
 *      Author: iizuka
 */

#include "bc_base_trx_index/TransactionData.h"

#include "bc_trx/AbstractBlockchainTransaction.h"

#include "bc_base/BinaryUtils.h"

#include "base/StackRelease.h"

namespace codablecash {

TransactionData::TransactionData(const AbstractBlockchainTransaction* trx) {
	this->trx = dynamic_cast<AbstractBlockchainTransaction*>(trx->copyData());
}

TransactionData::~TransactionData() {
	delete this->trx;
}

int TransactionData::binarySize() const {
	BinaryUtils::checkNotNull(this->trx);

	int total = this->trx->binarySize();

	return total;
}

void TransactionData::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->trx);

	this->trx->toBinary(out);

	__ASSERT_POS(out);
}

TransactionData* TransactionData::fromBinary(ByteBuffer *in) {
	AbstractBlockchainTransaction* trx = AbstractBlockchainTransaction::createFromBinary(in); __STP(trx);
	BinaryUtils::checkNotNull(trx);

	return new TransactionData(trx);
}

IBlockObject* TransactionData::copyData() const noexcept {
	return new TransactionData(this->trx);
}

} /* namespace codablecash */
