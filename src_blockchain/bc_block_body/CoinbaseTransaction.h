/*
 * CoinbaseTransaction.h
 *
 *  Created on: 2023/05/15
 *      Author: iizuka
 */

#ifndef BC_BLOCK_BODY_COINBASETRANSACTION_H_
#define BC_BLOCK_BODY_COINBASETRANSACTION_H_

#include "bc_block_body/AbstractBlockRewordTransaction.h"

namespace codablecash {

class Coinbase;

class CoinbaseTransaction : public AbstractBlockRewordTransaction {
public:
	CoinbaseTransaction(const CoinbaseTransaction& inst);
	CoinbaseTransaction();
	virtual ~CoinbaseTransaction();

	virtual uint8_t getType() const noexcept {
		return AbstractBlockchainTransaction::TRX_TYPE_COIN_BASE;
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

	void setCoinbase(const Coinbase* coinbase) noexcept;

	bool coinbaseEquals(const CoinbaseTransaction* otherTrx) const noexcept;

private:
	Coinbase* coinbase;

};

} /* namespace codablecash */

#endif /* BC_BLOCK_BODY_COINBASETRANSACTION_H_ */
