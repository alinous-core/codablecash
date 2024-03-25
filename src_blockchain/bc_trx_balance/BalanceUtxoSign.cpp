/*
 * BalanceUtxoSign.cpp
 *
 *  Created on: 2023/02/22
 *      Author: iizuka
 */

#include "bc_trx_balance/BalanceUtxoSign.h"

#include "ecda/Secp256k1Point.h"

#include "base/StackRelease.h"
namespace codablecash {

BalanceUtxoSign::BalanceUtxoSign(const BalanceUtxoSign &inst) : R(inst.R), s(inst.s) {

}

BalanceUtxoSign::BalanceUtxoSign(const Secp256k1CompressedPoint *R, const BigInteger *s) : R(*R), s(*s){
}

BalanceUtxoSign::~BalanceUtxoSign() {

}

int BalanceUtxoSign::binarySize() const {
	int total = this->R.binarySize();
	//total += s.binarySize();
	total += sizeof(uint8_t) * 32;

	return total;
}

void BalanceUtxoSign::toBinary(ByteBuffer *out) const {
	this->R.toBinary(out);

	ByteBuffer* tmp = this->s.toBinary(); __STP(tmp);
	tmp->position(0);
	ByteBuffer* bin32 = BigInteger::padBuffer(tmp, 32); __STP(bin32);
	bin32->position(0);
	out->put(bin32);
}

BalanceUtxoSign* BalanceUtxoSign::fromBinary(ByteBuffer *in) {
	Secp256k1CompressedPoint* pt = Secp256k1CompressedPoint::fromBinary(in); __STP(pt);

	uint8_t dest[32];
	in->get(dest, 32);
	BigInteger* bi = BigInteger::fromBinary((const char*)dest, 32); __STP(bi);

	return new BalanceUtxoSign(pt, bi);
}

IBlockObject* BalanceUtxoSign::copyData() const noexcept {
	return new BalanceUtxoSign(*this);
}

} /* namespace codablecash */
