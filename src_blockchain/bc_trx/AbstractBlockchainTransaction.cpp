/*
 * AbstractBlockchainTransaction.cpp
 *
 *  Created on: 2023/01/25
 *      Author: iizuka
 */

#include "bc_trx/AbstractBlockchainTransaction.h"
#include "bc_trx/TransactionId.h"
#include "bc_trx/AbstractUtxoReference.h"
#include "bc_trx/NopInterChainCommunicationTransaction.h"

#include "bc_trx_genesis/GenesisTransaction.h"

#include "bc_trx_balance/BalanceTransferTransaction.h"

#include "osenv/funcs.h"

#include "bc_finalizer_trx/RegisterVotePoolTransaction.h"
#include "bc_finalizer_trx/RegisterTicketTransaction.h"
#include "bc_finalizer_trx/VoteBlockTransaction.h"
#include "bc_finalizer_trx/RevokeMissedTicket.h"
#include "bc_finalizer_trx/RevokeMissVotedTicket.h"

#include "bc_status_cache_context/IStatusCacheContext.h"

#include "base/StackRelease.h"

#include "bc_memorypool/MemPoolTransaction.h"
#include "bc_memorypool/MempoolUtxoFindResult.h"

#include "bc_block_body/CoinbaseTransaction.h"
#include "bc_block_body/StakeBaseTransaction.h"

#include "bc_smartcontract/NopSmartcontractTransaction.h"

#include "base_timestamp/SystemTimestamp.h"

using alinous::Os;

namespace codablecash {

AbstractBlockchainTransaction::AbstractBlockchainTransaction(const AbstractBlockchainTransaction &inst) {
	this->trxId = inst.trxId != nullptr ? dynamic_cast<TransactionId*>(inst.trxId->copyData()) : nullptr;
	this->timestamp = dynamic_cast<SystemTimestamp*>(inst.timestamp->copyData());
}

AbstractBlockchainTransaction::AbstractBlockchainTransaction() {
	this->trxId = nullptr;
	this->timestamp = dynamic_cast<SystemTimestamp*>(Os::now().copyData());
}

AbstractBlockchainTransaction::~AbstractBlockchainTransaction() {
	delete this->trxId;
	delete this->timestamp;
}

AbstractBlockchainTransaction* AbstractBlockchainTransaction::createFromBinary(ByteBuffer *in) {
	AbstractBlockchainTransaction* ret = nullptr;

	uint8_t type = in->get();
	switch(type){
	case TRX_TYPE_GENESIS:
		ret = new GenesisTransaction();
		break;
	case TRX_TYPE_BANANCE_TRANSFER:
		ret = new BalanceTransferTransaction();
		break;
	case TRX_TYPE_REGISTER_VOTE_POOL:
		ret = new RegisterVotePoolTransaction();
		break;
	case TRX_TYPE_REGISTER_TICKET:
		ret = new RegisterTicketTransaction();
		break;
	case TRX_TYPE_VOTE_BLOCK:
		ret = new VoteBlockTransaction();
		break;
	case TRX_TYPE_REVOKE_MISSED_TICKET:
		ret = new RevokeMissedTicket();
		break;
	case TRX_TYPE_REVOKE_MISS_VOTED_TICKET:
		ret = new RevokeMissVotedTicket();
		break;
	case TRX_TYPE_COIN_BASE:
		ret = new CoinbaseTransaction();
		break;
	case TRX_TYPE_STAKE_BASE:
		ret = new StakeBaseTransaction();
		break;
	case TRX_TYPE_SMARTCONTRACT_NOP:
		ret = new NopSmartcontractTransaction();
		break;
	case TRX_TYPE_ICC_NOP:
		ret = new NopInterChainCommunicationTransaction();
		break;
	default:
		return nullptr;
	}

	ret->fromBinary(in);
	ret->build();

	return ret;
}

UtxoValidationResult AbstractBlockchainTransaction::validateUtxos(MemPoolTransaction *memTrx,	IStatusCacheContext *context, BalanceUnit fee) const {
	UtxoValidationResult result = UtxoValidationResult::ON_CHAIN;

	BalanceUnit leftBalance(0L);
	{
		int maxLoop = getUtxoReferenceSize();
		for(int i = 0; i != maxLoop; ++i){
			const AbstractUtxoReference* ref = getUtxoReference(i);
			const UtxoId* utxoId = ref->getUtxoId();

			// on chain
			{
				AbstractUtxo* utxo = context->getUtxo(utxoId); __STP(utxo);
				if(utxo != nullptr){
					leftBalance += utxo->getAmount();
					continue;
				}
			}

			// on memory pool
			if(memTrx != nullptr){
				MempoolUtxoFindResult* res = memTrx->findUtxo(utxoId); __STP(res);
				if(res != nullptr){
					const AbstractUtxo* utxo = res->getUtxo();
					leftBalance += utxo->getAmount();
					result = (result == UtxoValidationResult::ON_CHAIN) ? UtxoValidationResult::ON_MEMORY : result;
					continue;
				}
			}

			// no utxo
			return UtxoValidationResult::INVALID;
		}
	}

	BalanceUnit rightBalance = fee;
	{
		int maxLoop = getUtxoSize();
		for(int i = 0; i != maxLoop; ++i){
			const AbstractUtxo* utxo = getUtxo(i);

			rightBalance += utxo->getAmount();
		}
	}

	if(rightBalance.compareTo(&leftBalance) != 0){
		return UtxoValidationResult::INVALID;
	}

	return result;
}

TrxValidationResult AbstractBlockchainTransaction::validateReported(const BlockHeader *header, IStatusCacheContext *context) const {
	return validateFinal(header, nullptr, context);
}

} /* namespace codablecash */
