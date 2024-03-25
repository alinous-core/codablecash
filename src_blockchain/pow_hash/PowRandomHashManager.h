/*
 * PowRandomHashManager.h
 *
 *  Created on: 2023/03/09
 *      Author: iizuka
 */

#ifndef POW_HASH_POWRANDOMHASHMANAGER_H_
#define POW_HASH_POWRANDOMHASHMANAGER_H_

#include "base/ArrayList.h"

namespace alinous {
class ByteBuffer;
class BigInteger;
}
using namespace alinous;

namespace codablecash {

class PoWHashProgram;
class AstroBwt;
class PoWForLoop;
class AbstractHashAlgorithmElement;

class PowRandomHashManager {
public:
	PowRandomHashManager();
	virtual ~PowRandomHashManager();

	ByteBuffer* calculate(const BigInteger* hash, const BigInteger* nonce);


private:
	ByteBuffer* excecProgram(const BigInteger* hash, const BigInteger* nonce);

	void createProgram(const BigInteger* hash, const BigInteger* nonce);
	ArrayList<PoWForLoop>* creteForLoops(const BigInteger *nonce, int num, int totalNumOp, int totalLoops) const noexcept;
	PoWForLoop* buildForLoop(const BigInteger *nonce, int numOp, int loops) const noexcept;

	AbstractHashAlgorithmElement* getOperator(const BigInteger *nonce, int value) const noexcept;
private:
	PoWHashProgram* program;
	AstroBwt* abwt;
};

} /* namespace codablecash */

#endif /* POW_HASH_POWRANDOMHASHMANAGER_H_ */
