/*
 * SchnorrKeyPair.h
 *
 *  Created on: 2018/04/12
 *      Author: iizuka
 */

#ifndef CRYPTO_SCHNORRKEYPAIR_H_
#define CRYPTO_SCHNORRKEYPAIR_H_

#include <gmp.h>
#include "IKeyPair.h"

namespace alinous {
class BigInteger;
}

namespace codablecash {
using namespace alinous;

class SchnorrKeyPair : public IKeyPair {
private:
	friend class IKeyPair;
	SchnorrKeyPair();

public:

	SchnorrKeyPair(const SchnorrKeyPair& inst) = delete;
	SchnorrKeyPair(const BigInteger& secretKey, const BigInteger& publicKey);
	virtual ~SchnorrKeyPair();

	virtual IKeyPair* clone() const noexcept;

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual const BigInteger* getPubKey() const noexcept;
	virtual const BigInteger* getSecretKey() const noexcept;

	BigInteger* secretKey;
	BigInteger* publicKey;
};

} /* namespace codablecash */

#endif /* CRYPTO_SCHNORRKEYPAIR_H_ */
