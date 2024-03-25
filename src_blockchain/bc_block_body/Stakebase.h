/*
 * Stakebase.h
 *
 *  Created on: 2023/05/15
 *      Author: iizuka
 */

#ifndef BC_BLOCKSTORE_BODY_STAKEBASE_H_
#define BC_BLOCKSTORE_BODY_STAKEBASE_H_

#include "bc_trx/AbstractUtxoReference.h"

namespace codablecash {

class BalanceUnit;

class Stakebase : public AbstractUtxoReference {
public:
	Stakebase(const Stakebase& inst);
	Stakebase();
	virtual ~Stakebase();

	virtual uint8_t getType() const noexcept {
		return AbstractUtxoReference::UTXO_REF_TYPE_STAKEBASE;
	}

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

	void setAmount(const BalanceUnit* amount) noexcept;
	const BalanceUnit* getAmount() const noexcept {
		return this->amount;
	}

private:
	BalanceUnit* amount;

};

} /* namespace codablecash */

#endif /* BC_BLOCKSTORE_BODY_STAKEBASE_H_ */
