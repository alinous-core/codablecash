/*
 * StakeBaseTransaction.cpp
 *
 *  Created on: 2023/05/15
 *      Author: iizuka
 */

#include "bc_block_body/StakeBaseTransaction.h"
#include "bc_block_body/Stakebase.h"

#include "bc_base/BalanceUnit.h"
#include "bc_base/BinaryUtils.h"

#include "bc_trx/AbstractUtxoReference.h"
#include "bc_trx/TransactionId.h"
#include "bc_trx/AbstractUtxo.h"

#include "base/StackRelease.h"

#include "crypto/Sha256.h"

#include "bc_finalizer_trx/TicketVotedUtxoReference.h"
#include "bc_finalizer_trx/TicketVotedUtxo.h"

#include "bc_status_cache_context/IStatusCacheContext.h"


namespace codablecash {

StakeBaseTransaction::StakeBaseTransaction(const StakeBaseTransaction &inst) : AbstractBlockRewordTransaction(inst) {
	this->stakebase = inst.stakebase != nullptr ? dynamic_cast<Stakebase*>(inst.stakebase->copyData()) : nullptr;
}

StakeBaseTransaction::StakeBaseTransaction() : AbstractBlockRewordTransaction() {
	this->stakebase = nullptr;
}

StakeBaseTransaction::~StakeBaseTransaction() {
	delete this->stakebase;
}

int StakeBaseTransaction::binarySize() const {
	BinaryUtils::checkNotNull(this->stakebase);

	int total = __binarySize();
	total += this->stakebase->binarySize();

	return total;
}

void StakeBaseTransaction::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->stakebase);

	__toBinary(out);
	this->stakebase->toBinary(out);
}

void StakeBaseTransaction::fromBinary(ByteBuffer *in) {
	__fromBinary(in);

	AbstractUtxoReference* ref = AbstractUtxoReference::createFromBinary(in); __STP(ref);
	this->stakebase = dynamic_cast<Stakebase*>(ref);
	BinaryUtils::checkNotNull(this->stakebase);
	__STP_MV(ref);
}

IBlockObject* StakeBaseTransaction::copyData() const noexcept {
	return new StakeBaseTransaction(*this);
}

void StakeBaseTransaction::build() {
	setUtxoNonce();

	int capacity = binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(capacity, true); __STP(buff);

	toBinary(buff);
	__ASSERT_POS(buff);

	buff->position(0);

	ByteBuffer* sha = Sha256::sha256(buff, true); __STP(sha);

	delete this->trxId;
	this->trxId = new TransactionId((const char*)sha->array(), sha->limit());
}

int StakeBaseTransaction::getUtxoSize() const noexcept {
	return AbstractBlockRewordTransaction::getUtxoSize();
}

AbstractUtxo* StakeBaseTransaction::getUtxo(int i) const noexcept {
	return AbstractBlockRewordTransaction::getUtxo(i);
}

int StakeBaseTransaction::getUtxoReferenceSize() const noexcept {
	return AbstractBlockRewordTransaction::getUtxoReferenceSize() + 1;
}

AbstractUtxoReference* StakeBaseTransaction::getUtxoReference(int i) const noexcept {
	if(AbstractBlockRewordTransaction::getUtxoReferenceSize() == i){
		return this->stakebase;
	}

	return AbstractBlockRewordTransaction::getUtxoReference(i);
}

TicketVotedUtxoReference* StakeBaseTransaction::getTicketVotedUtxoReference() const noexcept {
	return dynamic_cast<TicketVotedUtxoReference*>(AbstractBlockRewordTransaction::getUtxoReference(0));
}

BalanceUnit StakeBaseTransaction::getFeeRate() const noexcept {
	return BalanceUnit(0L);
}

bool StakeBaseTransaction::validateOnAccept(MemPoolTransaction *memTrx,
		IStatusCacheContext *context) const {
	return true;
}

TrxValidationResult StakeBaseTransaction::validateFinal(const BlockHeader *header
		, MemPoolTransaction *memTrx, IStatusCacheContext *context) const {
	int size = getUtxoReferenceSize();

	BalanceUnit refTotal(0L);
	if(size == 2){
		const BalanceUnit* a = this->stakebase->getAmount();
		refTotal += *a;

		AbstractUtxoReference* ref = AbstractBlockRewordTransaction::getUtxoReference(0);
		const UtxoId* utxoId = ref->getUtxoId();

		AbstractUtxo* utxo = context->getUtxo(utxoId); __STP(utxo);
		TicketVotedUtxo* votedUtxo = dynamic_cast<TicketVotedUtxo*>(utxo);
		if(votedUtxo != nullptr){
			refTotal += votedUtxo->getAmount();
		}
	}

	BalanceUnit total(0L);
	{
		int maxLoop = getUtxoSize();
		for(int i = 0; i != maxLoop; ++i){
			AbstractUtxo* utxo = getUtxo(i);
			BalanceUnit amount = utxo->getAmount();

			total += amount;
		}
	}

	return total.compareTo(&refTotal) == 0 ? TrxValidationResult::OK : TrxValidationResult::INVALID;
}

void StakeBaseTransaction::setStakeBase(const Stakebase *stakebase) noexcept {
	delete this->stakebase;
	this->stakebase = dynamic_cast<Stakebase*>(stakebase->copyData());
}

} /* namespace codablecash */
