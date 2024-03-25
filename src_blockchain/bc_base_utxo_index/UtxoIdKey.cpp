/*
 * UtxoIdKey.cpp
 *
 *  Created on: 2023/02/24
 *      Author: iizuka
 */

#include "bc_base_utxo_index/UtxoIdKey.h"

#include "bc_trx/UtxoId.h"

#include "bc_base/BinaryUtils.h"

#include "bc_base_utxo_index/UtxoIdKeyFactory.h"

#include "base/StackRelease.h"
namespace codablecash {

UtxoIdKey::UtxoIdKey() {
	this->utxoId = nullptr;
}

UtxoIdKey::UtxoIdKey(const UtxoId* utxoId) {
	this->utxoId = dynamic_cast<UtxoId*>(utxoId->copyData());
}

UtxoIdKey::~UtxoIdKey() {
	delete this->utxoId;
}

int UtxoIdKey::binarySize() const {
	BinaryUtils::checkNotNull(this->utxoId);

	int size = sizeof(uint32_t);
	size += this->utxoId->binarySize();

	return size;
}

void UtxoIdKey::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->utxoId);

	out->putInt(UtxoIdKeyFactory::UTXO_ID_KEY);

	this->utxoId->toBinary(out);
}

UtxoIdKey* UtxoIdKey::fromBinary(ByteBuffer *in) {
	UtxoId* utxoId = UtxoId::fromBinary(in); __STP(utxoId);
	BinaryUtils::checkNotNull(utxoId);

	return new UtxoIdKey(utxoId);
}

int UtxoIdKey::compareTo(const AbstractBtreeKey *key) const noexcept {
	if(key->isInfinity()){
		return -1;
	}
	else if(key->isNull()){
		return 1;
	}

	const UtxoIdKey* other = dynamic_cast<const UtxoIdKey*>(key);
	return this->utxoId->compareTo(other->utxoId);
}

AbstractBtreeKey* UtxoIdKey::clone() const noexcept {
	return new UtxoIdKey(this->utxoId);
}

} /* namespace codablecash */
