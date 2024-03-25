/*
 * BalanceUtxoReference.cpp
 *
 *  Created on: 2023/02/20
 *      Author: iizuka
 */

#include "bc_trx_balance/BalanceUtxoReference.h"

#include "bc_base/BalanceAddress.h"
#include "bc_base/BalanceUnit.h"
#include "bc_base/BinaryUtils.h"

#include "base/StackRelease.h"

#include "bc_trx/UtxoId.h"

#include "ecda/Secp256k1CompressedPoint.h"

namespace codablecash {

BalanceUtxoReference::BalanceUtxoReference(const BalanceUtxoReference &inst)
		: AbstractUtxoReference(inst) {
	this->Xi = inst.Xi != nullptr ? dynamic_cast<Secp256k1CompressedPoint*>(inst.Xi->copyData()): nullptr;
}

BalanceUtxoReference::BalanceUtxoReference(): AbstractUtxoReference() {
	this->Xi = nullptr;
}

BalanceUtxoReference::~BalanceUtxoReference() {
	delete this->Xi;
}

int BalanceUtxoReference::binarySize() const {
	BinaryUtils::checkNotNull(this->Xi);

	int total = sizeof(uint8_t);
	total += this->utxoId->binarySize();
	total += this->Xi->binarySize();

	return total;
}

void BalanceUtxoReference::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->Xi);

	out->put(getType());
	this->utxoId->toBinary(out);
	this->Xi->toBinary(out);
}

void BalanceUtxoReference::fromBinary(ByteBuffer *in) {
	this->utxoId = UtxoId::fromBinary(in);

	Secp256k1CompressedPoint* adr = Secp256k1CompressedPoint::fromBinary(in); __STP(adr);
	this->Xi = dynamic_cast<Secp256k1CompressedPoint*>(adr);
	BinaryUtils::checkNotNull(this->Xi);

	__STP_MV(adr);
}

IBlockObject* BalanceUtxoReference::copyData() const noexcept {
	return new BalanceUtxoReference(*this);
}

void BalanceUtxoReference::setXi(const Secp256k1CompressedPoint *Xi) {
	delete this->Xi;
	this->Xi = new Secp256k1CompressedPoint(*Xi);
}

} /* namespace codablecash */
