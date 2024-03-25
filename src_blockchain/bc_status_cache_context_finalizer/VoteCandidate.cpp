/*
 * VoteCandidate.cpp
 *
 *  Created on: 2023/04/28
 *      Author: iizuka
 */

#include "bc_status_cache_context_finalizer/VoteCandidate.h"

#include "bc_trx/UtxoId.h"

#include "bc_network/NodeIdentifier.h"

#include "base/StackRelease.h"

#include "bc_base/AddressDescriptor.h"

#include "bc_base/BinaryUtils.h"
namespace codablecash {

VoteCandidate::VoteCandidate(const VoteCandidate &inst) {
	this->utxoId = new UtxoId(*inst.utxoId);
	this->nodeId = new NodeIdentifier(*inst.nodeId);
	this->voted = inst.voted;
	this->addressDesc = inst.addressDesc != nullptr ? dynamic_cast<AddressDescriptor*>(inst.addressDesc->copyData()) : nullptr;
	this->ticketPrice = inst.ticketPrice;

	this->voterAddressDesc = inst.voterAddressDesc != nullptr ? dynamic_cast<AddressDescriptor*>(inst.voterAddressDesc->copyData()) : nullptr;
}

VoteCandidate::VoteCandidate(const UtxoId* utxoId, const NodeIdentifier* nodeId) {
	this->utxoId = new UtxoId(*utxoId);
	this->nodeId = new NodeIdentifier(*nodeId);
	this->voted = false;
	this->addressDesc = nullptr;
	this->ticketPrice = BalanceUnit(0L);

	this->voterAddressDesc = nullptr;
}

VoteCandidate::~VoteCandidate() {
	delete this->utxoId;
	delete this->nodeId;
	delete this->addressDesc;

	delete this->voterAddressDesc;
}

int VoteCandidate::binarySize() const {
	BinaryUtils::checkNotNull(this->addressDesc);
	BinaryUtils::checkNotNull(this->voterAddressDesc);

	int total = this->utxoId->binarySize();
	total += this->nodeId->binarySize();
	total += sizeof(uint8_t);

	total += this->addressDesc->binarySize();
	total += this->ticketPrice.binarySize();

	total += this->voterAddressDesc->binarySize();

	return total;
}

void VoteCandidate::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->addressDesc);
	BinaryUtils::checkNotNull(this->voterAddressDesc);

	this->utxoId->toBinary(out);
	this->nodeId->toBinary(out);
	out->put(this->voted ? 1 : 0);

	this->addressDesc->toBinary(out);
	this->ticketPrice.toBinary(out);

	this->voterAddressDesc->toBinary(out);
}

VoteCandidate* VoteCandidate::createFromBinary(ByteBuffer *in) {
	UtxoId* utxoId = UtxoId::fromBinary(in); __STP(utxoId);
	NodeIdentifier* nodeId = NodeIdentifier::fromBinary(in); __STP(nodeId);
	uint8_t bl = in->get();

	VoteCandidate* ret = new VoteCandidate(utxoId, nodeId);
	ret->setVoted(bl > 0);

	ret->addressDesc = AddressDescriptor::createFromBinary(in);
	BinaryUtils::checkNotNull(ret->addressDesc);

	BalanceUnit* balance = BalanceUnit::fromBinary(in); __STP(balance);
	BinaryUtils::checkNotNull(balance);
	ret->ticketPrice = *balance;

	ret->voterAddressDesc = AddressDescriptor::createFromBinary(in);
	BinaryUtils::checkNotNull(ret->voterAddressDesc);

	return ret;
}

IBlockObject* VoteCandidate::copyData() const noexcept {
	return new VoteCandidate(*this);
}

void VoteCandidate::setAddressDescriptor(const AddressDescriptor *addressDesc) noexcept {
	delete this->addressDesc;
	this->addressDesc = new AddressDescriptor(*addressDesc);
}

void VoteCandidate::setTicketPrice(BalanceUnit ticketPrice) noexcept {
	this->ticketPrice = ticketPrice;
}

void VoteCandidate::setVoterAddressDescriptor(const AddressDescriptor *voterAddressDesc) noexcept {
	delete this->voterAddressDesc;
	this->voterAddressDesc = dynamic_cast<AddressDescriptor*>(voterAddressDesc->copyData());
}

} /* namespace codablecash */
