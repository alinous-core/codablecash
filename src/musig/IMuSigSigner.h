/*
 * IMuSigSigner.h
 *
 *  Created on: 2023/02/01
 *      Author: iizuka
 */

#ifndef MUSIG_IMUSIGSIGNER_H_
#define MUSIG_IMUSIGSIGNER_H_

namespace alinous {
class BigInteger;
}
using namespace alinous;

namespace codablecash {

class Secp256k1Point;

class IMuSigSigner {
public:
	IMuSigSigner();
	virtual ~IMuSigSigner();

	virtual Secp256k1Point getxG() = 0;
	virtual Secp256k1Point getrG() = 0;
	/**
	 * Each signer computes si = ri + H(X,R,m)H(L,Xi)xi
	 * HXRm: H(X,R,m)
	 */
	virtual BigInteger gets(const BigInteger* HXRm, const BigInteger* L) = 0;
};

} /* namespace codablecash */

#endif /* MUSIG_IMUSIGSIGNER_H_ */
