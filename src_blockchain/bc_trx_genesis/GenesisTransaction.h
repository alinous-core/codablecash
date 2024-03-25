/*
 * GenesisTransaction.h
 *
 *  Created on: 2023/01/25
 *      Author: iizuka
 */

#ifndef BC_TRX_GENESIS_GENESISTRANSACTION_H_
#define BC_TRX_GENESIS_GENESISTRANSACTION_H_

#include "bc_trx/AbstractBalanceTransaction.h"
#include "bc_trx_balance/BalanceUtxo.h"

#include "base/ArrayList.h"

using alinous::ArrayList;

namespace codablecash {

class BalanceUtxo;
class BalanceUnit;

class GenesisTransaction : public AbstractBalanceTransaction {
public:
	GenesisTransaction(const GenesisTransaction& inst);
	GenesisTransaction();
	virtual ~GenesisTransaction();

	virtual uint8_t getType() const noexcept {
		return AbstractBlockchainTransaction::TRX_TYPE_GENESIS;
	}

	void addBalanceUtxo(const BalanceUtxo* utxo) noexcept;

	BalanceUnit getTotalBalance() const noexcept;

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

	virtual void build();

	virtual int getUtxoSize() const noexcept {
		return this->list.size();
	}
	virtual AbstractUtxo* getUtxo(int i) const noexcept {
		return this->list.get(i);
	}

	virtual BalanceUnit getFeeRate() const noexcept;
	virtual BalanceUnit getFee() const noexcept;

	virtual int getUtxoReferenceSize() const noexcept;
	virtual AbstractUtxoReference* getUtxoReference(int i) const noexcept;

	virtual bool validateOnAccept(MemPoolTransaction *memTrx, IStatusCacheContext* context) const;
	virtual TrxValidationResult validateFinal(const BlockHeader* header, MemPoolTransaction *memTrx, IStatusCacheContext* context) const;

private:
	void setUtxoNonce() noexcept;


private:
	ArrayList<BalanceUtxo> list;

};

} /* namespace codablecash */

#endif /* BC_TRX_GENESIS_GENESISTRANSACTION_H_ */
