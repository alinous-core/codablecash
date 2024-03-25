/*
 * BlockRewardCalculator.h
 *
 *  Created on: 2023/05/03
 *      Author: iizuka
 */

#ifndef BC_BLOCK_GENERATOR_BLOCKREWARDCALCULATOR_H_
#define BC_BLOCK_GENERATOR_BLOCKREWARDCALCULATOR_H_

#include <cstdint>

#include "bc_base/BalanceUnit.h"

#include "base/ArrayList.h"

namespace codablecash {

class VotedHeaderIdGroup;
class VotingBlockStatus;
class BlockBody;
class BalanceUnit;
class BlockRewardStakeBase;
class BlockRewardMiningBase;
class MiningConfig;
class AddressDescriptor;
class CoinbaseTransaction;

class BlockRewardCalculator {
public:
	explicit BlockRewardCalculator(uint16_t numZones);
	virtual ~BlockRewardCalculator();

	void importPowConfig(const MiningConfig* miningConfig) noexcept;
	void setPoWConfig(const AddressDescriptor* desc) noexcept;
	void importVotes(const VotedHeaderIdGroup* group, const VotingBlockStatus* status, const BlockBody* body);

	void setFee(const BalanceUnit fee) noexcept {
		this->fee = fee;
	}

	void calcRewords(uint64_t height, uint16_t zone) noexcept;

	void exportCoinBaseTransaction(BlockBody* body, uint64_t height);
	void exportStakeBaseTransactions(BlockBody* body, uint64_t ticketVoterFeeBasisPoint, uint64_t height);

	CoinbaseTransaction* generateCoinbaseTransaction(uint64_t height) const;
	const ArrayList<BlockRewardStakeBase>* getStakeRewardBases() const noexcept {
		return &this->list;
	}

private:
	void exportStakeBaseTransaction(BlockBody* body, const BlockRewardStakeBase* base, uint64_t ticketVoterFeeBasisPoint, uint64_t height);

	BalanceUnit getTotalRewords(uint64_t height) const noexcept;
	BalanceUnit getTotalRewords4Shards(uint64_t height, uint16_t zone) const noexcept;


private:
	uint64_t numZones;
	BalanceUnit fee;

	ArrayList<BlockRewardStakeBase> list;
	BlockRewardMiningBase* pow;
};

} /* namespace codablecash */

#endif /* BC_BLOCK_GENERATOR_BLOCKREWARDCALCULATOR_H_ */
