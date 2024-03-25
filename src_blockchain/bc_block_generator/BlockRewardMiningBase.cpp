/*
 * BlockReworkMiningBase.cpp
 *
 *  Created on: 2023/05/16
 *      Author: iizuka
 */

#include "bc_base/AddressDescriptor.h"

#include "bc_block_body/Coinbase.h"

#include "base/StackRelease.h"
#include "bc_block_generator/BlockRewardMiningBase.h"

#include "bc_trx_balance/BalanceUtxo.h"
namespace codablecash {

BlockRewardMiningBase::BlockRewardMiningBase(const AddressDescriptor* addressDesc) {
	this->addressDesc = dynamic_cast<AddressDescriptor*>(addressDesc->copyData());
}

BlockRewardMiningBase::~BlockRewardMiningBase() {
	delete this->addressDesc;
}

void BlockRewardMiningBase::setReward(const BalanceUnit reward) noexcept {
	this->reward = reward;
}

Coinbase* BlockRewardMiningBase::toCoinbase() const noexcept {
	Coinbase* coinbase = new Coinbase(); __STP(coinbase);
	coinbase->setAmount(&this->reward);

	return __STP_MV(coinbase);
}

BalanceUtxo* BlockRewardMiningBase::toBalanceUtxo() const {
	BalanceUnit amount = this->reward;

	BalanceUtxo* utxo = new BalanceUtxo(amount); __STP(utxo);
	utxo->setAddress(this->addressDesc);

	return __STP_MV(utxo);
}

} /* namespace codablecash */
