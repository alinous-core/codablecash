/*
 * SimpleMuSigSigner.h
 *
 *  Created on: 2023/02/02
 *      Author: iizuka
 */

#ifndef MUSIG_SIMPLEMUSIGSIGNER_H_
#define MUSIG_SIMPLEMUSIGSIGNER_H_

#include "musig/IMuSigSigner.h"

#include "numeric/BigInteger.h"

namespace codablecash {

class SimpleMuSigSigner : public IMuSigSigner {
public:
	explicit SimpleMuSigSigner(const BigInteger x);
	virtual ~SimpleMuSigSigner();

	virtual Secp256k1Point getxG();
	virtual Secp256k1Point getrG();
	virtual BigInteger gets(const BigInteger* HXRm, const BigInteger* L);

private:
	BigInteger x;
	BigInteger r;
};

} /* namespace codablecash */

#endif /* MUSIG_SIMPLEMUSIGSIGNER_H_ */
