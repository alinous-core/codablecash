/*
 * Coinbase.cpp
 *
 *  Created on: 2023/05/15
 *      Author: iizuka
 */

#include "bc_block_body/Coinbase.h"

#include "bc_base/BalanceUnit.h"
#include "bc_base/BinaryUtils.h"

namespace codablecash {

Coinbase::Coinbase(const Coinbase &inst) : AbstractUtxoReference(inst) {
	this->amount = inst.amount != nullptr ? dynamic_cast<BalanceUnit*>(inst.amount->copyData()) : nullptr;
}

Coinbase::Coinbase() : AbstractUtxoReference() {
	this->amount = nullptr;
}

Coinbase::~Coinbase() {
	delete this->amount;
}

int Coinbase::binarySize() const {
	BinaryUtils::checkNotNull(this->amount);

	int total = sizeof(uint8_t);
	total += this->amount->binarySize();

	return total;
}

void Coinbase::toBinary(ByteBuffer *out) const {
	out->put(getType());
	this->amount->toBinary(out);
}

void Coinbase::fromBinary(ByteBuffer *in) {
	this->amount = BalanceUnit::fromBinary(in);
	BinaryUtils::checkNotNull(this->amount);
}

IBlockObject* Coinbase::copyData() const noexcept {
	return new Coinbase(*this);
}

void Coinbase::setAmount(const BalanceUnit *amount) noexcept {
	delete this->amount;
	this->amount = dynamic_cast<BalanceUnit*>(amount->copyData());
}

bool Coinbase::equals(const Coinbase *other) const noexcept {
	return this->amount->compareTo(other->amount) == 0;
}

bool Coinbase::checkFilter(const ArrayList<BloomFilter1024> *filtersList) const {
	return false;
}

} /* namespace codablecash */
