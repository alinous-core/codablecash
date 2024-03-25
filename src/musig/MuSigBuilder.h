/*
 * MuSigBuilder.h
 *
 *  Created on: 2023/02/01
 *      Author: iizuka
 */

#ifndef MUSIG_MUSIGBUILDER_H_
#define MUSIG_MUSIGBUILDER_H_

#include "base/ArrayList.h"
#include "ecda/Secp256k1Point.h"

namespace alinous {
class BigInteger;
}
using namespace alinous;

namespace codablecash {

class IMuSigSigner;
class Secp256k1Point;
class MuSig;

class MuSigBuilder {
public:
	MuSigBuilder();
	virtual ~MuSigBuilder();

	void addSigner(IMuSigSigner* signer) noexcept;

	MuSig sign(const char* data, int length);

	ArrayList<IMuSigSigner>* getSigners() const noexcept {
		return this->signers;
	}

private:
	void calcL();
	void calcX();
	void calcR();
	void calcs(const char *data, int length);

private:
	ArrayList<IMuSigSigner>* signers;
	BigInteger L;
	ArrayList<Secp256k1Point>* XiList;

	Secp256k1Point X;
	Secp256k1Point R;
	BigInteger s;
};

} /* namespace codablecash */

#endif /* MUSIG_MUSIGBUILDER_H_ */
