/*
 * NopSmartcontractTransaction.h
 *
 *  Created on: 2023/12/04
 *      Author: iizuka
 */

#ifndef BC_SMARTCONTRACT_NOPSMARTCONTRACTTRANSACTION_H_
#define BC_SMARTCONTRACT_NOPSMARTCONTRACTTRANSACTION_H_

#include "bc_smartcontract/AbstractSmartcontractTransaction.h"

namespace codablecash {

class NopSmartcontractTransaction : public AbstractSmartcontractTransaction {
public:
	NopSmartcontractTransaction(const NopSmartcontractTransaction& inst);
	NopSmartcontractTransaction();
	virtual ~NopSmartcontractTransaction();

	virtual uint8_t getType() const noexcept;

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual void build();

	virtual IBlockObject* copyData() const noexcept;

	virtual BalanceUnit getFee() const noexcept;
	virtual BalanceUnit getFeeRate() const noexcept;

	virtual int getUtxoSize() const noexcept;
	virtual AbstractUtxo* getUtxo(int i) const noexcept;
	virtual int getUtxoReferenceSize() const noexcept;
	virtual AbstractUtxoReference* getUtxoReference(int i) const noexcept;


	virtual bool validateOnAccept(MemPoolTransaction *memTrx, IStatusCacheContext* context) const;
	virtual TrxValidationResult validateFinal(const BlockHeader* header, MemPoolTransaction *memTrx, IStatusCacheContext* context) const;

private:
	uint64_t nonce;

	static uint64_t serial;
};

} /* namespace codablecash */

#endif /* BC_SMARTCONTRACT_NOPSMARTCONTRACTTRANSACTION_H_ */
