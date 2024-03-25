/*
 * BlockRewardCalculator.cpp
 *
 *  Created on: 2023/05/03
 *      Author: iizuka
 */

#include "bc_block_generator/BlockRewardCalculator.h"
#include "bc_block_generator/MiningConfig.h"

#include "bc_block_vote/VotedHeaderIdGroup.h"

#include "bc_status_cache_context_finalizer/VotingBlockStatus.h"
#include "bc_status_cache_context_finalizer/VoteCandidate.h"

#include "bc_block_body/BlockBody.h"
#include "bc_block_body/StakeBaseTransaction.h"
#include "bc_block_body/CoinbaseTransaction.h"

#include "bc_finalizer_trx/VoteBlockTransaction.h"
#include "bc_finalizer_trx/TicketVotedUtxo.h"

#include "base/StackRelease.h"

#include "bc_block_body/Stakebase.h"

#include "bc_finalizer_trx/TicketVotedUtxoReference.h"

#include "bc_trx_balance/BalanceUtxo.h"

#include "bc_block_body/Coinbase.h"

#include "bc/ExceptionThrower.h"
#include "bc_block_generator/BlockRewardMiningBase.h"
#include "bc_block_generator/BlockRewardStakeBase.h"

#include "bc_block_validator/BlockValidationException.h"
namespace codablecash {

BlockRewardCalculator::BlockRewardCalculator(uint16_t numZones) : fee(0L) {
	this->numZones = numZones;
	this->pow = nullptr;
}

BlockRewardCalculator::~BlockRewardCalculator() {
	this->list.deleteElements();
	delete this->pow;
}

void BlockRewardCalculator::importPowConfig(const MiningConfig *miningConfig) noexcept {
	const AddressDescriptor* desc = miningConfig->getAddressDescriptor();

	if(desc != nullptr){
		this->pow = new BlockRewardMiningBase(desc);
	}
}

void BlockRewardCalculator::setPoWConfig(const AddressDescriptor *desc) noexcept {
	this->pow = new BlockRewardMiningBase(desc);
}

void BlockRewardCalculator::importVotes(const VotedHeaderIdGroup *group,	const VotingBlockStatus *status, const BlockBody* body) {
	int maxLoop = group->size();
	for(int i = 0; i != maxLoop; ++i){
		const TransactionId* trxId = group->get(i);
		const AbstractBlockchainTransaction* trx = body->getControlTransaction(trxId);

		const VoteBlockTransaction* voteTrx = dynamic_cast<const VoteBlockTransaction*>(trx);
		if(voteTrx != nullptr){
			const TicketVotedUtxo* ticketVotedVotedUtxo = voteTrx->getTicketVotedUtxo();
			const UtxoId* votedUtxoId = ticketVotedVotedUtxo->getVotedUtxoId();

			const VoteCandidate* candidate = status->getVoteCandidate(votedUtxoId);

			//assert(candidate != nullptr);
			ExceptionThrower<BlockValidationException>::throwExceptionIfCondition(candidate == nullptr
					, L"VotedUtxoId in Vote candidate is wrong", __FILE__, __LINE__);

			BlockRewardStakeBase* base = new BlockRewardStakeBase(); __STP(base);
			base->setNodeIdentifier(candidate->getNodeIdentifier());
			base->setTicketVotedUtxo(ticketVotedVotedUtxo);
			base->setAddressDescriptor(candidate->getAddressDescriptor());
			base->setTicketPrice(candidate->getTicletPrice());
			base->setVoterAddressDesc(candidate->getVoterAddressDescriptor());

			this->list.addElement(__STP_MV(base));
		}
	}
}

void BlockRewardCalculator::calcRewords(uint64_t height, uint16_t zone) noexcept {
	BalanceUnit total = getTotalRewords4Shards(height, zone) + this->fee;

	int totalShare = this->list.size();
	if(this->pow != nullptr){
		totalShare++;
	}

	if(totalShare == 0){
		return;
	}

	BalanceUnit perShare = total / totalShare;
	BalanceUnit remain = total - (perShare * BalanceUnit(totalShare));

	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		BlockRewardStakeBase* stake = this->list.get(i);
		stake->setReward(perShare);
	}

	if(this->pow != nullptr){
		BalanceUnit powShare = perShare + remain;
		this->pow->setReward(powShare);
	}

}

BalanceUnit BlockRewardCalculator::getTotalRewords(uint64_t height) const noexcept {
	return BalanceUnit(100000L);
}

BalanceUnit BlockRewardCalculator::getTotalRewords4Shards(uint64_t height, uint16_t zone) const noexcept {
	BalanceUnit uintTotal = getTotalRewords(height);

	BalanceUnit perShard = uintTotal / this->numZones;
	BalanceUnit remain = uintTotal - (perShard * BalanceUnit(this->numZones));

	if(zone == 0){
		perShard += remain;
	}

	return perShard;
}

CoinbaseTransaction* BlockRewardCalculator::generateCoinbaseTransaction(uint64_t height) const {
	CoinbaseTransaction* trx = new CoinbaseTransaction(); __STP(trx);
	trx->setHeight(height);

	Coinbase* coinbase = this->pow->toCoinbase(); __STP(coinbase);
	trx->setCoinbase(coinbase);

	BalanceUtxo* utxo = this->pow->toBalanceUtxo(); __STP(utxo);
	trx->addUtxo(utxo);

	trx->build();

	return __STP_MV(trx);
}

void BlockRewardCalculator::exportCoinBaseTransaction(BlockBody *body, uint64_t height) {
	if(this->pow != nullptr){
		CoinbaseTransaction* trx = new CoinbaseTransaction(); __STP(trx);
		trx->setHeight(height);

		Coinbase* coinbase = this->pow->toCoinbase(); __STP(coinbase);
		trx->setCoinbase(coinbase);

		BalanceUtxo* utxo = this->pow->toBalanceUtxo(); __STP(utxo);
		trx->addUtxo(utxo);

		trx->build();
		body->addBlockRewordTransaction(trx);

#ifdef __DEBUG__
		assert(trx->validateOnAccept(nullptr, nullptr) == true);
		assert(trx->validateFinal(nullptr, nullptr, nullptr) == TrxValidationResult::OK);

		BalanceUnit zero(0L);
		assert(trx->getFeeRate().compareTo(&zero) == 0);
		assert(trx->getFee().compareTo(&zero) == 0);
#endif
	}
}

void BlockRewardCalculator::exportStakeBaseTransactions(BlockBody *body, uint64_t ticketVoterFeeBasisPoint, uint64_t height) {
	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		BlockRewardStakeBase* base = this->list.get(i);

		exportStakeBaseTransaction(body, base, ticketVoterFeeBasisPoint, height);
	}
}

void BlockRewardCalculator::exportStakeBaseTransaction(BlockBody *body, const BlockRewardStakeBase *base, uint64_t ticketVoterFeeBasisPoint, uint64_t height) {
	StakeBaseTransaction* trx = new StakeBaseTransaction(); __STP(trx);
	trx->setHeight(height);

	Stakebase* stakebase = base->toStakebase(); __STP(stakebase);
	trx->setStakeBase(stakebase);

	TicketVotedUtxoReference* ref = base->toTicketVotedUtxoReference(); __STP(ref);
	trx->addInputUtxoRef(ref);

	BalanceUtxo* utxo = base->toBalanceUtxo(ticketVoterFeeBasisPoint); __STP(utxo);
	trx->addUtxo(utxo);

	BalanceUtxo* voterFeeUtxo = base->toVoterFeeBalanceUtxo(ticketVoterFeeBasisPoint); __STP(voterFeeUtxo);
	trx->addUtxo(voterFeeUtxo);

	trx->build();

	body->addBlockRewordTransaction(trx);

#ifdef __DEBUG__
	assert(trx->validateOnAccept(nullptr, nullptr) == true);
	//assert(trx->validateFinal(nullptr, nullptr, nullptr) == TrxValidationResult::OK);

	BalanceUnit zero(0L);
	assert(trx->getFeeRate().compareTo(&zero) == 0);
#endif
}

} /* namespace codablecash */
