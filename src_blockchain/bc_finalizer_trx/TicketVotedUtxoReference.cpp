/*
 * TicketVotedUtxoReference.cpp
 *
 *  Created on: 2023/05/15
 *      Author: iizuka
 */

#include "bc_finalizer_trx/TicketVotedUtxoReference.h"

#include "bc_trx/UtxoId.h"
namespace codablecash {

TicketVotedUtxoReference::TicketVotedUtxoReference(const TicketVotedUtxoReference &inst)
		: AbstractUtxoReference(inst) {
}

TicketVotedUtxoReference::TicketVotedUtxoReference() {

}

TicketVotedUtxoReference::~TicketVotedUtxoReference() {

}

int TicketVotedUtxoReference::binarySize() const {
	int total = sizeof(uint8_t);
	total += this->utxoId->binarySize();

	return total;
}

void TicketVotedUtxoReference::toBinary(ByteBuffer *out) const {
	out->put(getType());
	this->utxoId->toBinary(out);
}

void TicketVotedUtxoReference::fromBinary(ByteBuffer *in) {
	this->utxoId = UtxoId::fromBinary(in);
}

IBlockObject* TicketVotedUtxoReference::copyData() const noexcept {
	return new TicketVotedUtxoReference(*this);
}

} /* namespace codablecash */
