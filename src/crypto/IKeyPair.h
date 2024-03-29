/*
 * IKeyPair.h
 *
 *  Created on: 2019/01/25
 *      Author: iizuka
 */

#ifndef CRYPTO_IKEYPAIR_H_
#define CRYPTO_IKEYPAIR_H_
#include <cstdlib>


namespace alinous {
class ByteBuffer;
class BigInteger;
}

namespace codablecash {
using namespace alinous;

class IKeyPair {
public:
	static const constexpr int8_t PAIR_SCHNORR{1};

	virtual ~IKeyPair();

	virtual IKeyPair* clone() const noexcept = 0;
	virtual void toBinary(ByteBuffer* out) const = 0;
	virtual int binarySize() const = 0;
	virtual void fromBinary(ByteBuffer* in) = 0;

	virtual const BigInteger* getPubKey() const noexcept = 0;
	virtual const BigInteger* getSecretKey() const noexcept = 0;

	static IKeyPair* createFromBinary(ByteBuffer* in);
};

} /* namespace codablecash */

#endif /* CRYPTO_IKEYPAIR_H_ */
