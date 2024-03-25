/*
 * BlockRewordStakeBase.cpp
 *
 *  Created on: 2023/05/16
 *      Author: iizuka
 */

#include "bc_finalizer_trx/TicketVotedUtxo.h"
#include "bc_finalizer_trx/TicketVotedUtxoReference.h"

#include "bc_base/AddressDescriptor.h"

#include "bc_block_body/Stakebase.h"

#include "base/StackRelease.h"

#include "bc_trx_balance/BalanceUtxo.h"

#include "bc_network/NodeIdentifier.h"

#include "bc_base/BalanceAddress.h"
#include "bc_block_generator/BlockRewardStakeBase.h"
namespace codablecash {

BlockRewardStakeBase::BlockRewardStakeBase() : ticketPrice(0L), reward(0L) {
	this->ticketVotedUtxo = nullptr;
	this->addressDesc = nullptr;
	this->nodeId = nullptr;
	this->voterAddressDesc = nullptr;
}

BlockRewardStakeBase::~BlockRewardStakeBase() {
	delete this->ticketVotedUtxo;
	delete this->addressDesc;
	delete this->nodeId;
	delete this->voterAddressDesc;
}

void BlockRewardStakeBase::setNodeIdentifier(	const NodeIdentifier *nodeId) noexcept {
	delete this->nodeId;
	this->nodeId = dynamic_cast<NodeIdentifier*>(nodeId->copyData());
}

void BlockRewardStakeBase::setAddressDescriptor(const AddressDescriptor *addressDesc) noexcept {
	delete this->addressDesc;
	this->addressDesc = dynamic_cast<AddressDescriptor*>(addressDesc->copyData());
}

void BlockRewardStakeBase::setTicketVotedUtxo(const TicketVotedUtxo *ticketVotedUtxo) noexcept {
	delete this->ticketVotedUtxo;
	this->ticketVotedUtxo = dynamic_cast<TicketVotedUtxo*>(ticketVotedUtxo->copyData());
}

void BlockRewardStakeBase::setTicketPrice(const BalanceUnit ticketPrice) noexcept {
	this->ticketPrice = ticketPrice;
}

void BlockRewardStakeBase::setReward(const BalanceUnit reward) noexcept {
	this->reward = reward;
}

Stakebase* BlockRewardStakeBase::toStakebase() const {
	Stakebase* base = new Stakebase(); __STP(base);
	base->setAmount(&this->reward);

	return __STP_MV(base);
}

TicketVotedUtxoReference* BlockRewardStakeBase::toTicketVotedUtxoReference() const {
	TicketVotedUtxoReference* ref = new TicketVotedUtxoReference(); __STP(ref);

	const UtxoId* utxoId = this->ticketVotedUtxo->getId();
	ref->setUtxoId(utxoId);

	return __STP_MV(ref);
}

BalanceUtxo* BlockRewardStakeBase::toBalanceUtxo(uint64_t ticketVoterFeeBasisPoint) const {
	BalanceUnit amount = calcTicketOwnerBalance(ticketVoterFeeBasisPoint);

	BalanceUtxo* utxo = new BalanceUtxo(amount); __STP(utxo);

	utxo->setAddress(this->addressDesc);

	return __STP_MV(utxo);
}

BalanceUnit BlockRewardStakeBase::calcTicketOwnerBalance(uint64_t ticketVoterFeeBasisPoint) const {
	BalanceUnit voterReword = this->reward * BalanceUnit(ticketVoterFeeBasisPoint) / 10000L;
	BalanceUnit ticketReword = this->reward - voterReword;

	BalanceUnit amount = ticketReword + this->ticketVotedUtxo->getAmount();

	return amount;
}

void BlockRewardStakeBase::setVoterAddressDesc(const AddressDescriptor *voterAddressDesc) noexcept {
	delete this->voterAddressDesc;
	this->voterAddressDesc = dynamic_cast<AddressDescriptor*>(voterAddressDesc->copyData());
}

BalanceUtxo* BlockRewardStakeBase::toVoterFeeBalanceUtxo(uint64_t ticketVoterFeeBasisPoint) const {
	BalanceUnit amount = calcVoterFeeBalance(ticketVoterFeeBasisPoint);

	BalanceUtxo* utxo = new BalanceUtxo(amount); __STP(utxo);

	utxo->setAddress(this->voterAddressDesc);

	return __STP_MV(utxo);
}

BalanceUnit BlockRewardStakeBase::calcVoterFeeBalance(uint64_t ticketVoterFeeBasisPoint) const {
	BalanceUnit voterReword = this->reward * BalanceUnit(ticketVoterFeeBasisPoint) / 10000L;

	return voterReword;
}

} /* namespace codablecash */
