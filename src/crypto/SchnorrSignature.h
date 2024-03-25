/*
 * SchnorrSignature.h
 *
 *  Created on: 2018/04/12
 *      Author: iizuka
 */

#ifndef CRYPTO_SCHNORRSIGNATURE_H_
#define CRYPTO_SCHNORRSIGNATURE_H_

#include <gmp.h>

namespace alinous {
class BigInteger;
class ByteBuffer;
}

namespace codablecash {
using namespace alinous;

class SchnorrSignature {
public:
	BigInteger* e, *y;

	SchnorrSignature(const SchnorrSignature& inst);
	SchnorrSignature(const BigInteger& e, const BigInteger& y);
	virtual ~SchnorrSignature();

	int binarySize() const;
	void toBinary(ByteBuffer* out);
	static SchnorrSignature* createFromBinary(ByteBuffer* in);

private:
	static BigInteger* loadBinary(ByteBuffer* in);
};

} /* namespace codablecash */

#endif /* CRYPTO_SCHNORRSIGNATURE_H_ */
