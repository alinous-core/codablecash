/*
 * TotalNumberSplitter.h
 *
 *  Created on: 2023/03/10
 *      Author: iizuka
 */

#ifndef POW_HASH_TOTALNUMBERSPLITTER_H_
#define POW_HASH_TOTALNUMBERSPLITTER_H_

#include "base/RawArrayPrimitive.h"

namespace alinous {
class BigInteger;
}
using namespace alinous;

namespace codablecash {

class TotalNumberSplitter {
public:
	TotalNumberSplitter(const BigInteger* nonce, int num, int totalNumOp);
	virtual ~TotalNumberSplitter();

	int get(int i) const noexcept {
		return this->list.get(i);
	}

private:
	void add2Position(int pos, int diff) noexcept;

private:
	RawArrayPrimitive<int> list;
};

} /* namespace codablecash */

#endif /* POW_HASH_TOTALNUMBERSPLITTER_H_ */
