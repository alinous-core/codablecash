/*
 * BalanceUtxoReference.h
 *
 *  Created on: 2023/02/20
 *      Author: iizuka
 */

#ifndef BC_TRX_BALANCE_BALANCEUTXOREFERENCE_H_
#define BC_TRX_BALANCE_BALANCEUTXOREFERENCE_H_

#include "bc_trx/AbstractUtxoReference.h"

#include "bc_base/BalanceUnit.h"

namespace codablecash {

class Secp256k1CompressedPoint;
class UtxoId;

class BalanceUtxoReference: public AbstractUtxoReference {
public:
	BalanceUtxoReference(const BalanceUtxoReference& inst);
	BalanceUtxoReference();
	virtual ~BalanceUtxoReference();

	virtual uint8_t getType() const noexcept {
		return AbstractUtxoReference::UTXO_REF_TYPE_BALANCE;
	}

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

	void setXi(const Secp256k1CompressedPoint* Xi);

	const Secp256k1CompressedPoint* getXi() const noexcept {
		return this->Xi;
	}
private:
	Secp256k1CompressedPoint* Xi;
};

} /* namespace codablecash */

#endif /* BC_TRX_BALANCE_BALANCEUTXOREFERENCE_H_ */
