/*
 * Coinbase.h
 *
 *  Created on: 2023/05/15
 *      Author: iizuka
 */

#ifndef BC_BLOCKSTORE_BODY_COINBASE_H_
#define BC_BLOCKSTORE_BODY_COINBASE_H_

#include "bc_trx/AbstractUtxoReference.h"

namespace codablecash {

class BalanceUnit;

class Coinbase : public AbstractUtxoReference {
public:
	Coinbase(const Coinbase& inst);
	Coinbase();
	virtual ~Coinbase();

	virtual uint8_t getType() const noexcept {
		return AbstractUtxoReference::UTXO_REF_TYPE_COINBASE;
	}

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

	void setAmount(const BalanceUnit* amount) noexcept;
	const BalanceUnit* getAmount() const noexcept {
		return this->amount;
	}

	bool equals(const Coinbase* other) const noexcept;

private:
	BalanceUnit* amount;
};

} /* namespace codablecash */

#endif /* BC_BLOCKSTORE_BODY_COINBASE_H_ */
