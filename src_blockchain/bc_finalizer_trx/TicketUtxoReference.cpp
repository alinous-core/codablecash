/*
 * TicketUtxoReference.cpp
 *
 *  Created on: 2023/05/03
 *      Author: iizuka
 */

#include "bc_finalizer_trx/TicketUtxoReference.h"

#include "bc_trx/UtxoId.h"

#include "bc_base/BinaryUtils.h"

#include "bc_wallet_filter/BloomHash1024.h"

namespace codablecash {

TicketUtxoReference::TicketUtxoReference(const TicketUtxoReference &inst)
		: AbstractUtxoReference(inst) {

}

TicketUtxoReference::TicketUtxoReference() : AbstractUtxoReference() {

}

TicketUtxoReference::~TicketUtxoReference() {

}

int TicketUtxoReference::binarySize() const {
	BinaryUtils::checkNotNull(this->utxoId);
	BinaryUtils::checkNotNull(this->bloomHash);

	int total = sizeof(uint8_t);
	total += this->utxoId->binarySize();

	total += this->bloomHash->binarySize();

	return total;
}

void TicketUtxoReference::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->utxoId);
	BinaryUtils::checkNotNull(this->bloomHash);

	out->put(getType());
	this->utxoId->toBinary(out);

	this->bloomHash->toBinary(out);
}

void TicketUtxoReference::fromBinary(ByteBuffer *in) {
	this->utxoId = UtxoId::fromBinary(in);

	this->bloomHash = BloomHash1024::createFromBinary(in);

	BinaryUtils::checkNotNull(this->utxoId);
	BinaryUtils::checkNotNull(this->bloomHash);
}

IBlockObject* TicketUtxoReference::copyData() const noexcept {
	return new TicketUtxoReference(*this);
}

} /* namespace codablecash */
