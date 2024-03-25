/*
 * NopInterChainCommunicationTransaction.h
 *
 *  Created on: 2023/12/04
 *      Author: iizuka
 */

#ifndef BC_TRX_NOPINTERCHAINCOMMUNICATIONTRANSACTION_H_
#define BC_TRX_NOPINTERCHAINCOMMUNICATIONTRANSACTION_H_

#include "bc_trx/AbstractInterChainCommunicationTansaction.h"


namespace codablecash {

class NopInterChainCommunicationTransaction : public AbstractInterChainCommunicationTansaction {
public:
	NopInterChainCommunicationTransaction(const NopInterChainCommunicationTransaction& inst);
	NopInterChainCommunicationTransaction();
	virtual ~NopInterChainCommunicationTransaction();

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

#endif /* BC_TRX_NOPINTERCHAINCOMMUNICATIONTRANSACTION_H_ */
