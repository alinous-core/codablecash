/*
 * ScPrivateKey.h
 *
 *  Created on: 2022/03/23
 *      Author: iizuka
 */

#ifndef ECDA_SCPRIVATEKEY_H_
#define ECDA_SCPRIVATEKEY_H_

#include "numeric/BigInteger.h"

#include "Secp256k1Point.h"

using namespace alinous;

namespace codablecash {

class ScPublicKey;

class ScPrivateKey {
public:
	static const BigInteger p; // = new BigInteger("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F", 16);

	ScPrivateKey(const BigInteger* seed, uint64_t solt);
	ScPrivateKey();
	virtual ~ScPrivateKey();

	ScPublicKey generatePublicKey();

	const BigInteger getKeyvalue() const noexcept {
		return keyvalue;
	}

private:
	BigInteger keyvalue;

};

} /* namespace codablecash */

#endif /* ECDA_SCPRIVATEKEY_H_ */
