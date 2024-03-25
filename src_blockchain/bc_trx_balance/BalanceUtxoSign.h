/*
 * BalanceUtxoSign.h
 *
 *  Created on: 2023/02/22
 *      Author: iizuka
 */

#ifndef BC_TRX_BALANCE_BALANCEUTXOSIGN_H_
#define BC_TRX_BALANCE_BALANCEUTXOSIGN_H_

#include "numeric/BigInteger.h"

#include "base/ArrayList.h"

#include "filestore_block/IBlockObject.h"

#include "ecda/Secp256k1CompressedPoint.h"

namespace codablecash {

class BalanceUtxoSign : public alinous::IBlockObject {
public:
	BalanceUtxoSign(const BalanceUtxoSign& inst);
	BalanceUtxoSign(const Secp256k1CompressedPoint* R, const BigInteger* s);
	virtual ~BalanceUtxoSign();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	static BalanceUtxoSign* fromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

	const Secp256k1CompressedPoint* getR() const noexcept {
		return &this->R;
	}
	const BigInteger* gets() const noexcept {
		return &this->s;
	}
private:
	Secp256k1CompressedPoint R;
	BigInteger s;
};

} /* namespace codablecash */

#endif /* BC_TRX_BALANCE_BALANCEUTXOSIGN_H_ */
