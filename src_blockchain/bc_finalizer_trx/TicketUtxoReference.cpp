/*
 * TicketUtxoReference.cpp
 *
 *  Created on: 2023/05/03
 *      Author: iizuka
 */

#include "bc_finalizer_trx/TicketUtxoReference.h"

#include "bc_trx/UtxoId.h"
namespace codablecash {

TicketUtxoReference::TicketUtxoReference(const TicketUtxoReference &inst)
		: AbstractUtxoReference(inst) {

}

TicketUtxoReference::TicketUtxoReference() : AbstractUtxoReference() {

}

TicketUtxoReference::~TicketUtxoReference() {

}

int TicketUtxoReference::binarySize() const {
	int total = sizeof(uint8_t);
	total += this->utxoId->binarySize();

	return total;
}

void TicketUtxoReference::toBinary(ByteBuffer *out) const {
	out->put(getType());
	this->utxoId->toBinary(out);
}

void TicketUtxoReference::fromBinary(ByteBuffer *in) {
	this->utxoId = UtxoId::fromBinary(in);
}

IBlockObject* TicketUtxoReference::copyData() const noexcept {
	return new TicketUtxoReference(*this);
}

} /* namespace codablecash */
