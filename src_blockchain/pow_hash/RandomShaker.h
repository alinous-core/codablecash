/*
 * RandomShaker.h
 *
 *  Created on: 2023/03/09
 *      Author: iizuka
 */

#ifndef POW_HASH_RANDOMSHAKER_H_
#define POW_HASH_RANDOMSHAKER_H_

#include "base/RawArrayPrimitive.h"

namespace alinous {
class BigInteger;
}
using namespace alinous;

namespace codablecash {

class RandomShaker {
public:
	RandomShaker(const BigInteger* nonce, int length);
	virtual ~RandomShaker();

	int get(int i) noexcept;
private:
	RawArrayPrimitive<int> list;
};

} /* namespace codablecash */

#endif /* POW_HASH_RANDOMSHAKER_H_ */
