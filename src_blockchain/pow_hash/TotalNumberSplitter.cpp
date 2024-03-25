/*
 * TotalNumberSplitter.cpp
 *
 *  Created on: 2023/03/10
 *      Author: iizuka
 */

#include "pow_hash/TotalNumberSplitter.h"

#include "numeric/BigInteger.h"

#include <cstdlib>

namespace codablecash {

TotalNumberSplitter::TotalNumberSplitter(const BigInteger* nonce, int num, int totalNumOp) : list(num) {
	// init
	{
		for(int i = 0; i != num; ++i){
			this->list.set(i, 0);
		}
	}

	// calc
	int remain = totalNumOp;
	int maxValue = totalNumOp / num;

	int i = 0;
	while(remain > 0){
		int pos = i % num;

		int shift = i % 224;
		int partValue = nonce->shiftRight(shift).modui(maxValue).longValue();

		int diff = partValue <= remain ? partValue : remain;

		add2Position(pos, diff);
		remain -= diff;

		++i;
	}
}

TotalNumberSplitter::~TotalNumberSplitter() {

}

void TotalNumberSplitter::add2Position(int pos, int diff) noexcept {
	int last = this->list.get(pos);
	this->list.set(pos, last + diff);
}

} /* namespace codablecash */
