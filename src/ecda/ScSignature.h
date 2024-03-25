/*
 * ScSignature.h
 *
 *  Created on: 2022/03/23
 *      Author: iizuka
 */

#ifndef ECDA_SCSIGNATURE_H_
#define ECDA_SCSIGNATURE_H_

#include "Secp256k1Point.h"
#include "numeric/BigInteger.h"

namespace codablecash {

class ScSignature {
public:
	ScSignature(const ScSignature& inst);
	ScSignature();
	virtual ~ScSignature();

	void sign(ByteBuffer* data, const BigInteger s);

	bool verify(ByteBuffer* data, const Secp256k1Point& pubKey);

private:
	ByteBuffer* hash(ByteBuffer* buff);

private:
	BigInteger e;
	BigInteger y;
};

} /* namespace codablecash */

#endif /* ECDA_SCSIGNATURE_H_ */
