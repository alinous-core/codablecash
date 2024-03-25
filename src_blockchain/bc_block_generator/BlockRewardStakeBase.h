/*
 * BlockRewordStakeBase.h
 *
 *  Created on: 2023/05/16
 *      Author: iizuka
 */

#ifndef BC_BLOCK_GENERATOR_BLOCKREWARDSTAKEBASE_H_
#define BC_BLOCK_GENERATOR_BLOCKREWARDSTAKEBASE_H_

#include "bc_base/BalanceUnit.h"

namespace codablecash {

class TicketVotedUtxo;
class AddressDescriptor;
class Stakebase;
class TicketVotedUtxoReference;
class BalanceUtxo;
class NodeIdentifier;

class BlockRewardStakeBase {
public:
	BlockRewardStakeBase();
	virtual ~BlockRewardStakeBase();

	void setNodeIdentifier(const NodeIdentifier* nodeId) noexcept;
	void setAddressDescriptor(const AddressDescriptor* addressDesc) noexcept;
	void setTicketVotedUtxo(const TicketVotedUtxo* ticketVotedUtxo) noexcept;
	void setTicketPrice(const BalanceUnit ticketPrice) noexcept;
	void setVoterAddressDesc(const AddressDescriptor* voterAddressDesc) noexcept;

	void setReward(const BalanceUnit reward) noexcept;

	Stakebase* toStakebase() const;
	TicketVotedUtxoReference* toTicketVotedUtxoReference() const;
	BalanceUtxo* toBalanceUtxo(uint64_t ticketVoterFeeBasisPoint) const;
	BalanceUtxo* toVoterFeeBalanceUtxo(uint64_t ticketVoterFeeBasisPoint) const;

	BalanceUnit calcTicketOwnerBalance(uint64_t ticketVoterFeeBasisPoint) const;
	BalanceUnit calcVoterFeeBalance(uint64_t ticketVoterFeeBasisPoint) const;


	BalanceUnit getReward() const noexcept {
		return this->reward;
	}

	BalanceUnit getTicketPrice() const noexcept {
		return this->ticketPrice;
	}
	TicketVotedUtxo* getTicketVotedUtxo() const noexcept {
		return this->ticketVotedUtxo;
	}

	const AddressDescriptor* getAddressDescriptor() const noexcept {
		return this->addressDesc;
	}
	const AddressDescriptor* getVoterAddressDescriptor() const noexcept {
		return this->voterAddressDesc;
	}

private:
	NodeIdentifier* nodeId;

	BalanceUnit ticketPrice;
	TicketVotedUtxo* ticketVotedUtxo;
	AddressDescriptor* addressDesc; // ticket owner

	AddressDescriptor* voterAddressDesc; // ticket pool owner

	BalanceUnit reward;
};

} /* namespace codablecash */

#endif /* BC_BLOCK_GENERATOR_BLOCKREWARDSTAKEBASE_H_ */
