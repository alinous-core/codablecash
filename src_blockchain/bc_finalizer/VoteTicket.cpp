/*
 * VoteTicket.cpp
 *
 *  Created on: 2023/04/27
 *      Author: iizuka
 */

#include "bc_finalizer/VoteTicket.h"

#include "bc_network/NodeIdentifier.h"

#include "bc_base/AddressDescriptor.h"
#include "bc_base/BinaryUtils.h"

#include "bc_finalizer_trx/TicketUtxo.h"

#include "bc_trx/UtxoId.h"

#include "base/StackRelease.h"

namespace codablecash {

VoteTicket::VoteTicket(const VoteTicket &inst) {
	this->height = inst.height;
	this->nodeId = inst.nodeId != nullptr ? new NodeIdentifier(*inst.nodeId) : nullptr;
	this->utxoId = inst.utxoId != nullptr ? dynamic_cast<UtxoId*>(inst.utxoId->copyData()) : nullptr;
	this->addressDesc = inst.addressDesc != nullptr ? dynamic_cast<AddressDescriptor*>(inst.addressDesc->copyData()) : nullptr;
	this->voterAddressDesc = inst.voterAddressDesc != nullptr ? dynamic_cast<AddressDescriptor*>(inst.voterAddressDesc->copyData()) : nullptr;
	this->ticketPrice = inst.ticketPrice;
}

VoteTicket::VoteTicket() : ticketPrice(0L) {
	this->height = 0;
	this->nodeId = nullptr;
	this->utxoId = nullptr;
	this->addressDesc = nullptr;
	this->voterAddressDesc = nullptr;
}

VoteTicket::~VoteTicket() {
	delete this->nodeId;
	delete this->utxoId;
	delete this->addressDesc;
	delete this->voterAddressDesc;
}

VoteTicket* VoteTicket::toVoteTicket(uint64_t height, const TicketUtxo *ticketUtxo, const AddressDescriptor* voterAddressDesc) {
	assert(voterAddressDesc != nullptr);

	VoteTicket* ticket = new VoteTicket();
	ticket->setHeight(height);
	ticket->setUtxoId(ticketUtxo->getId());
	ticket->setNodeIdentifier(ticketUtxo->getNodeIdentifier());
	ticket->setAddressDesc(ticketUtxo->getAddress());
	ticket->setTicketPrice(ticketUtxo->getAmount());
	ticket->setVoterAddressDesc(voterAddressDesc);

	return ticket;
}

void VoteTicket::setNodeIdentifier(const NodeIdentifier *nodeId) noexcept {
	delete this->nodeId;
	this->nodeId = new NodeIdentifier(*nodeId);
}

void VoteTicket::setUtxoId(const UtxoId *utxoId) noexcept {
	delete this->utxoId;
	this->utxoId = dynamic_cast<UtxoId*>(utxoId->copyData());
}

void VoteTicket::setAddressDesc(const AddressDescriptor *addressDesc) noexcept {
	delete this->addressDesc;
	this->addressDesc = dynamic_cast<AddressDescriptor*>(addressDesc->copyData());
}

void VoteTicket::setTicketPrice(BalanceUnit ticketPrice) {
	this->ticketPrice = ticketPrice;
}

int VoteTicket::binarySize() const {
	BinaryUtils::checkNotNull(this->utxoId);
	BinaryUtils::checkNotNull(this->nodeId);
	BinaryUtils::checkNotNull(this->addressDesc);
	BinaryUtils::checkNotNull(this->voterAddressDesc);

	int total = sizeof(this->height);
	total += this->utxoId->binarySize();
	total += this->nodeId->binarySize();
	total += this->addressDesc->binarySize();
	total += this->ticketPrice.binarySize();
	total += this->voterAddressDesc->binarySize();

	return total;
}

void VoteTicket::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->utxoId);
	BinaryUtils::checkNotNull(this->nodeId);
	BinaryUtils::checkNotNull(this->addressDesc);
	BinaryUtils::checkNotNull(this->voterAddressDesc);

	out->putLong(this->height);
	this->utxoId->toBinary(out);
	this->nodeId->toBinary(out);
	this->addressDesc->toBinary(out);
	this->ticketPrice.toBinary(out);
	this->voterAddressDesc->toBinary(out);
}

VoteTicket* VoteTicket::createFromBinary(ByteBuffer *in) {
	VoteTicket* ticket = new VoteTicket(); __STP(ticket);

	ticket->height = in->getLong();
	ticket->utxoId = UtxoId::fromBinary(in);
	BinaryUtils::checkNotNull(ticket->utxoId);
	ticket->nodeId = NodeIdentifier::fromBinary(in);
	BinaryUtils::checkNotNull(ticket->nodeId);
	ticket->addressDesc = AddressDescriptor::createFromBinary(in);
	BinaryUtils::checkNotNull(ticket->addressDesc);

	BalanceUnit* balance = BalanceUnit::fromBinary(in); __STP(balance);
	ticket->ticketPrice = *balance;

	ticket->voterAddressDesc = AddressDescriptor::createFromBinary(in);
	BinaryUtils::checkNotNull(ticket->voterAddressDesc);

	return __STP_MV(ticket);
}

void VoteTicket::setVoterAddressDesc(const AddressDescriptor *voterAddressDesc) {
	delete this->voterAddressDesc;
	this->voterAddressDesc = dynamic_cast<AddressDescriptor*>(voterAddressDesc->copyData());
}

} /* namespace codablecash */
