/*
 * RandomShaker.cpp
 *
 *  Created on: 2023/03/09
 *      Author: iizuka
 */

#include "pow_hash/RandomShaker.h"

#include "numeric/BigInteger.h"

namespace codablecash {

RandomShaker::RandomShaker(const BigInteger* nonce, int length) : list(length) {
	for(int i = 0; i != length; ++i){
		int size = this->list.size();
		int64_t index = nonce->shiftRight(i).modui(size + 1).longValue();
		this->list.addElement(i, index);
	}
}

RandomShaker::~RandomShaker() {

}

int RandomShaker::get(int i) noexcept {
	return this->list.get(i);
}

} /* namespace codablecash */
