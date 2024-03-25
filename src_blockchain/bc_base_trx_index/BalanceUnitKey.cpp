/*
 * BalanceUnitKey.cpp
 *
 *  Created on: 2023/02/27
 *      Author: iizuka
 */

#include "bc_base_trx_index/BalanceUnitKey.h"
#include "bc_base_trx_index/BalanceUnitKeyFactory.h"

#include "bc_base/BalanceUnit.h"
#include "bc_base/BinaryUtils.h"

#include "base/StackRelease.h"

#include "bc_trx/TransactionId.h"


namespace codablecash {

BalanceUnitKey::BalanceUnitKey(const BalanceUnit* unit) {
	this->unit = dynamic_cast<BalanceUnit*>(unit->copyData());
	this->trxId = nullptr;
}

BalanceUnitKey::~BalanceUnitKey() {
	delete this->unit;
	delete this->trxId;
}

int BalanceUnitKey::binarySize() const {
	BinaryUtils::checkNotNull(this->unit);

	int size = sizeof(uint32_t);
	size += this->unit->binarySize();

	return size;
}

void BalanceUnitKey::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->unit);

	out->putInt(BalanceUnitKeyFactory::BALANCE_UNIT_KEY);

	this->unit->toBinary(out);
}

BalanceUnitKey* BalanceUnitKey::fromBinary(ByteBuffer *in) {
	BalanceUnit* unit = BalanceUnit::fromBinary(in); __STP(unit);
	BinaryUtils::checkNotNull(unit);

	return new BalanceUnitKey(unit);
}

int BalanceUnitKey::compareTo(const AbstractBtreeKey *key) const noexcept {
	if(key->isInfinity()){
		return -1;
	}
	else if(key->isNull()){
		return 1;
	}

	const BalanceUnitKey* other = dynamic_cast<const BalanceUnitKey*>(key);
	return this->unit->compareTo(other->unit);
}

AbstractBtreeKey* BalanceUnitKey::clone() const noexcept {
	BalanceUnitKey* key = new BalanceUnitKey(this->unit);
	return key;
}

void BalanceUnitKey::setTransactionId(const TransactionId *trxId) noexcept {
	delete this->trxId;
	this->trxId = dynamic_cast<TransactionId*>(trxId->copyData());
}

} /* namespace codablecash */
