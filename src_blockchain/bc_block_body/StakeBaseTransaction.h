/*
 * StakeBaseTransaction.h
 *
 *  Created on: 2023/05/15
 *      Author: iizuka
 */

#ifndef BC_BLOCK_BODY_STAKEBASETRANSACTION_H_
#define BC_BLOCK_BODY_STAKEBASETRANSACTION_H_

#include "bc_block_body/AbstractBlockRewordTransaction.h"

namespace codablecash {

class Stakebase;
class TicketVotedUtxoReference;

class StakeBaseTransaction : public AbstractBlockRewordTransaction {
public:
	StakeBaseTransaction(const StakeBaseTransaction& inst);
	StakeBaseTransaction();
	virtual ~StakeBaseTransaction();

	virtual uint8_t getType() const noexcept {
		return AbstractBlockchainTransaction::TRX_TYPE_STAKE_BASE;
	}

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

	virtual void build();

	virtual int getUtxoSize() const noexcept;
	virtual AbstractUtxo* getUtxo(int i) const noexcept;

	virtual int getUtxoReferenceSize() const noexcept;
	virtual AbstractUtxoReference* getUtxoReference(int i) const noexcept;

	virtual BalanceUnit getFeeRate() const noexcept;

	virtual bool validateOnAccept(MemPoolTransaction *memTrx, IStatusCacheContext* context) const;
	virtual TrxValidationResult validateFinal(const BlockHeader* header, MemPoolTransaction *memTrx, IStatusCacheContext* context) const;

	void setStakeBase(const Stakebase* stakebase) noexcept;
	const Stakebase* getStakebase() const noexcept {
		return this->stakebase;
	}

	TicketVotedUtxoReference* getTicketVotedUtxoReference() const noexcept;
private:
	Stakebase* stakebase;
};

} /* namespace codablecash */

#endif /* BC_BLOCK_BODY_STAKEBASETRANSACTION_H_ */
