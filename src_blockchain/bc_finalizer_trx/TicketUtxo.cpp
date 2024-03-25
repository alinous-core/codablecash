/*
 * TicketUtxo.cpp
 *
 *  Created on: 2023/04/27
 *      Author: iizuka
 */

#include "bc_finalizer_trx/TicketUtxo.h"

#include "bc_network/NodeIdentifier.h"

#include "bc_base/AddressDescriptor.h"

#include "bc_base/BinaryUtils.h"

#include "base/StackRelease.h"

#include "crypto/Sha256.h"

#include "bc_trx/UtxoId.h"
namespace codablecash {

TicketUtxo::TicketUtxo(const TicketUtxo &inst) : AbstractUtxo(inst), amount(inst.amount) {
	this->nodeId = inst.nodeId != nullptr ? new NodeIdentifier(*inst.nodeId) : nullptr;
	this->addressDesc = inst.addressDesc != nullptr ? new AddressDescriptor(*inst.addressDesc) : nullptr;
}

TicketUtxo::TicketUtxo() {
	this->nodeId = nullptr;
	this->amount = BalanceUnit(0L);
	this->addressDesc = nullptr;
}

TicketUtxo::~TicketUtxo() {
	delete this->nodeId;
	delete this->addressDesc;
}

uint8_t TicketUtxo::getType() const noexcept {
	return AbstractUtxo::TRX_UTXO_TICKET;
}


void TicketUtxo::build() {
	int capacity = this->nodeId->binarySize();
	capacity += this->addressDesc->binarySize();
	capacity += this->amount.binarySize();
	capacity += 32;

	ByteBuffer* buff = ByteBuffer::allocateWithEndian(capacity, true); __STP(buff);

	this->nodeId->toBinary(buff);
	this->addressDesc->toBinary(buff);
	this->amount.toBinary(buff);
	buff->put(this->nonce, 32);
	buff->position(0);

	ByteBuffer* sha = Sha256::sha256(buff, true); __STP(sha);

	delete this->utxoId;
	this->utxoId = new UtxoId((const char*)sha->array(), sha->limit());
}

const AddressDescriptor* TicketUtxo::getAddress() const noexcept {
	return this->addressDesc;
}

int TicketUtxo::binarySize() const {
	BinaryUtils::checkNotNull(this->utxoId);
	BinaryUtils::checkNotNull(this->addressDesc);
	BinaryUtils::checkNotNull(this->nodeId);

	int total = sizeof(uint8_t);

	total += this->utxoId->binarySize();

	total += this->nodeId->binarySize();
	total += this->addressDesc->binarySize();
	total += this->amount.binarySize();

	return total;
}

void TicketUtxo::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->utxoId);
	BinaryUtils::checkNotNull(this->addressDesc);
	BinaryUtils::checkNotNull(this->nodeId);

	out->put(getType());

	this->utxoId->toBinary(out);

	this->nodeId->toBinary(out);
	this->addressDesc->toBinary(out);
	this->amount.toBinary(out);
}

void TicketUtxo::fromBinary(ByteBuffer *in) {
	this->utxoId = UtxoId::fromBinary(in);

	this->nodeId = NodeIdentifier::fromBinary(in);

	this->addressDesc = AddressDescriptor::createFromBinary(in);

	BalanceUnit* unit = BalanceUnit::fromBinary(in); __STP(unit);
	this->amount = *unit;
}

BalanceUnit TicketUtxo::getAmount() const noexcept {
	return this->amount;
}

IBlockObject* TicketUtxo::copyData() const noexcept {
	return new TicketUtxo(*this);
}

void TicketUtxo::setNodeIndentifier(const NodeIdentifier *nodeId) noexcept {
	delete this->nodeId;
	this->nodeId = new NodeIdentifier(*nodeId);
}

void TicketUtxo::setAddressDescriptor(	const AddressDescriptor *addressDesc) noexcept {
	delete this->addressDesc;
	this->addressDesc = dynamic_cast<AddressDescriptor*>(addressDesc->copyData());
}

void TicketUtxo::setAmounst(BalanceUnit amount) noexcept {
	this->amount = amount;
}

} /* namespace codablecash */
