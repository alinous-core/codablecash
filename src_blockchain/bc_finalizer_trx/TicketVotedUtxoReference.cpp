/*
 * TicketVotedUtxoReference.cpp
 *
 *  Created on: 2023/05/15
 *      Author: iizuka
 */

#include "bc_finalizer_trx/TicketVotedUtxoReference.h"

#include "bc_trx/UtxoId.h"

#include "bc_base/BinaryUtils.h"

#include "bc_wallet_filter/BloomHash1024.h"

namespace codablecash {

TicketVotedUtxoReference::TicketVotedUtxoReference(const TicketVotedUtxoReference &inst)
		: AbstractUtxoReference(inst) {
}

TicketVotedUtxoReference::TicketVotedUtxoReference() {

}

TicketVotedUtxoReference::~TicketVotedUtxoReference() {

}

int TicketVotedUtxoReference::binarySize() const {
	BinaryUtils::checkNotNull(this->utxoId);
	BinaryUtils::checkNotNull(this->bloomHash);

	int total = sizeof(uint8_t);
	total += this->utxoId->binarySize();

	total += this->bloomHash->binarySize();

	return total;
}

void TicketVotedUtxoReference::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->utxoId);
	BinaryUtils::checkNotNull(this->bloomHash);

	out->put(getType());
	this->utxoId->toBinary(out);

	this->bloomHash->toBinary(out);
}

void TicketVotedUtxoReference::fromBinary(ByteBuffer *in) {
	this->utxoId = UtxoId::fromBinary(in);

	this->bloomHash = BloomHash1024::createFromBinary(in);

	BinaryUtils::checkNotNull(this->utxoId);
	BinaryUtils::checkNotNull(this->bloomHash);
}

IBlockObject* TicketVotedUtxoReference::copyData() const noexcept {
	return new TicketVotedUtxoReference(*this);
}

} /* namespace codablecash */
