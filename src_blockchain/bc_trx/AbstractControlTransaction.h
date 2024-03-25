/*
 * AbstractControlTransaction.h
 *
 *  Created on: 2023/04/15
 *      Author: iizuka
 */

#ifndef BC_TRX_ABSTRACTCONTROLTRANSACTION_H_
#define BC_TRX_ABSTRACTCONTROLTRANSACTION_H_

#include "bc_trx/AbstractBlockchainTransaction.h"

#include "bc_base/BalanceUnit.h"

#include "base/ArrayList.h"

namespace codablecash {

class InputUtxoCollection;
class BalanceUtxo;
class BalanceUnit;
class BalanceUtxoSign;
class BalanceUtxoReference;
class IMuSigSignerProvidor;
class IUtxoFinder;
class AbstractUtxo;

class AbstractControlTransaction : public AbstractBlockchainTransaction {
public:
	static const constexpr int MAX_INPUT_ELEMENT{256};

	AbstractControlTransaction(const AbstractControlTransaction& inst);
	AbstractControlTransaction();
	virtual ~AbstractControlTransaction();

	void addInputUtxoRef(const BalanceUtxoReference* ref) noexcept;
	void addBalanceUtxo(const BalanceUtxo* utxo) noexcept;

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual void setFeeAmount(const BalanceUnit *fee);

	void sign(IMuSigSignerProvidor *providor, IUtxoFinder* finder);
	bool verify() const;

	virtual BalanceUnit getFeeRate() const noexcept;
	virtual BalanceUnit getFee() const noexcept;

	virtual int getUtxoSize() const noexcept;
	virtual AbstractUtxo* getUtxo(int i) const noexcept;

	virtual int getUtxoReferenceSize() const noexcept;
	virtual AbstractUtxoReference* getUtxoReference(int i) const noexcept;

protected:
	void setUtxoNonce() noexcept;


	int __binarySize() const;
	void __toBinary(ByteBuffer* out) const;
	void __fromBinary(ByteBuffer* in);

protected:
	InputUtxoCollection* inputs;
	ArrayList<BalanceUtxo> list;
	BalanceUnit fee;
	BalanceUtxoSign* signature;
};

} /* namespace codablecash */

#endif /* BC_TRX_ABSTRACTCONTROLTRANSACTION_H_ */
