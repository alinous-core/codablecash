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

#include "bc_wallet_filter/BloomHash1024.h"


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
	BinaryUtils::checkNotNull(this->utxoId);
	BinaryUtils::checkNotNull(this->bloomHash);

	int total = sizeof(uint8_t);
	total += this->utxoId->binarySize();
	total += this->Xi->binarySize();

	total += this->bloomHash->binarySize();

	return total;
}

void BalanceUtxoReference::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->Xi);
	BinaryUtils::checkNotNull(this->utxoId);
	BinaryUtils::checkNotNull(this->bloomHash);

	out->put(getType());
	this->utxoId->toBinary(out);
	this->Xi->toBinary(out);

	this->bloomHash->toBinary(out);
}

void BalanceUtxoReference::fromBinary(ByteBuffer *in) {
	this->utxoId = UtxoId::fromBinary(in);

	Secp256k1CompressedPoint* adr = Secp256k1CompressedPoint::fromBinary(in); __STP(adr);
	this->Xi = dynamic_cast<Secp256k1CompressedPoint*>(adr);

	this->bloomHash = BloomHash1024::createFromBinary(in);

	BinaryUtils::checkNotNull(this->Xi);
	BinaryUtils::checkNotNull(this->bloomHash);

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
