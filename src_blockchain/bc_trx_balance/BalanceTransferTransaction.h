/*
 * BalanceTransferTransaction.h
 *
 *  Created on: 2023/01/25
 *      Author: iizuka
 */

#ifndef BC_TRX_BALANCE_BALANCETRANSFERTRANSACTION_H_
#define BC_TRX_BALANCE_BALANCETRANSFERTRANSACTION_H_

#include "bc_trx/AbstractBalanceTransaction.h"

#include "base/ArrayList.h"

#include "bc_base/BalanceUnit.h"

#include "bc_trx_balance/BalanceUtxo.h"

namespace codablecash {

class InputUtxoCollection;
class BalanceUtxoReference;
class BalanceUnit;
class BalanceUtxoSign;
class IMuSigSignerProvidor;
class IUtxoFinder;
class AddressDescriptor;
class BalanceUtxo;

class BalanceTransferTransaction : public AbstractBalanceTransaction {
public:
	static const constexpr int MAX_INPUT_ELEMENT{2048};

	BalanceTransferTransaction(const BalanceTransferTransaction& inst);
	BalanceTransferTransaction();
	virtual ~BalanceTransferTransaction();

	virtual uint8_t getType() const noexcept {
		return AbstractBlockchainTransaction::TRX_TYPE_BANANCE_TRANSFER;
	}

	void addInputUtxoRef(const BalanceUtxoReference* ref) noexcept;
	void addBalanceUtxo(const BalanceUtxo* utxo) noexcept;

	void setFeeAmount(const BalanceUnit* fee) noexcept;
	void setFeeAmount(const BalanceUnit* fee, const AddressDescriptor *skipfeedesc);
	virtual BalanceUnit getFeeRate() const noexcept;
	virtual BalanceUnit getFee() const noexcept;

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

	virtual void build();

	void sign(IMuSigSignerProvidor* providor, IUtxoFinder* finder);
	bool verify() const;

	virtual int getUtxoSize() const noexcept {
		return this->list.size();
	}
	virtual AbstractUtxo* getUtxo(int i) const noexcept {
		return this->list.get(i);
	}

	const ArrayList<BalanceUtxoReference>* getUtxoRefList() const noexcept;

	virtual int getUtxoReferenceSize() const noexcept;
	virtual AbstractUtxoReference* getUtxoReference(int i) const noexcept;

	virtual bool validateOnAccept(MemPoolTransaction *memTrx, IStatusCacheContext* context) const;
	virtual TrxValidationResult validateFinal(const BlockHeader* header, MemPoolTransaction *memTrx, IStatusCacheContext* context) const;

	BalanceUnit getTotalOutputBalance() const;

private:
	void setUtxoNonce() noexcept;

	virtual int __binarySize() const;
	virtual void __toBinary(ByteBuffer* out) const;

private:
	InputUtxoCollection* inputs;
	ArrayList<BalanceUtxo> list;
	BalanceUnit fee;

	BalanceUtxoSign* signature;
};

} /* namespace codablecash */

#endif /* BC_TRX_BALANCE_BALANCETRANSFERTRANSACTION_H_ */
