/*
 * BlockReworkMiningBase.h
 *
 *  Created on: 2023/05/16
 *      Author: iizuka
 */

#ifndef BC_BLOCK_GENERATOR_BLOCKREWARDMININGBASE_H_
#define BC_BLOCK_GENERATOR_BLOCKREWARDMININGBASE_H_

#include "bc_base/BalanceUnit.h"

namespace codablecash {

class AddressDescriptor;
class Coinbase;
class BalanceUtxo;

class BlockRewardMiningBase {
public:
	explicit BlockRewardMiningBase(const AddressDescriptor* addressDesc);
	virtual ~BlockRewardMiningBase();

	void setReward(const BalanceUnit reward) noexcept;

	Coinbase* toCoinbase() const noexcept;
	BalanceUtxo* toBalanceUtxo() const;
private:
	AddressDescriptor* addressDesc;
	BalanceUnit reward;
};

} /* namespace codablecash */

#endif /* BC_BLOCK_GENERATOR_BLOCKREWARDMININGBASE_H_ */
