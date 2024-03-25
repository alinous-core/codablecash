/*
 * RemovedDummyUtxo.cpp
 *
 *  Created on: 2023/05/12
 *      Author: iizuka
 */

#include "bc_status_cache_context/RemovedDummyUtxo.h"

#include "bc_base/BalanceUnit.h"

#include "bc_base/BinaryUtils.h"

#include "bc_trx/UtxoId.h"
namespace codablecash {

RemovedDummyUtxo::RemovedDummyUtxo(const RemovedDummyUtxo &inst) : AbstractUtxo(inst) {
}

RemovedDummyUtxo::RemovedDummyUtxo() : AbstractUtxo() {

}

RemovedDummyUtxo::~RemovedDummyUtxo() {

}

uint8_t RemovedDummyUtxo::getType() const noexcept {
	return AbstractUtxo::TRX_UTXO_REMOVED_DUMMY;
}

int RemovedDummyUtxo::binarySize() const {
	BinaryUtils::checkNotNull(this->utxoId);

	int total = sizeof(uint8_t);

	total += this->utxoId->binarySize();

	return total;
}

void RemovedDummyUtxo::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->utxoId);

	out->put(getType());

	this->utxoId->toBinary(out);
}

void RemovedDummyUtxo::fromBinary(ByteBuffer *in) {
	this->utxoId = UtxoId::fromBinary(in);
}

void RemovedDummyUtxo::build() {
}

IBlockObject* RemovedDummyUtxo::copyData() const noexcept {
	return new RemovedDummyUtxo(*this);
}

const AddressDescriptor* RemovedDummyUtxo::getAddress() const noexcept {
	return nullptr;
}

BalanceUnit RemovedDummyUtxo::getAmount() const noexcept {
	return BalanceUnit(0L);
}

void RemovedDummyUtxo::setUtxoId(const UtxoId *utxoId) noexcept {
	delete this->utxoId;
	this->utxoId = dynamic_cast<UtxoId*>(utxoId->copyData());
}

} /* namespace codablecash */
