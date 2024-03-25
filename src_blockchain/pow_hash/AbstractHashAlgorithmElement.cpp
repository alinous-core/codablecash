/*
 * AbstractHashAlgorithmElement.cpp
 *
 *  Created on: 2023/03/09
 *      Author: iizuka
 */

#include "pow_hash/AbstractHashAlgorithmElement.h"
#include "pow_hash/RandomShaker.h"

#include "numeric/BigInteger.h"


namespace codablecash {

AbstractHashAlgorithmElement::AbstractHashAlgorithmElement() {
	this->arg01 = 0;
	this->arg02 = 0;
	this->arg03 = 0;
	this->out01 = 0;
}

AbstractHashAlgorithmElement::~AbstractHashAlgorithmElement() {

}

void AbstractHashAlgorithmElement::buildInputs(const BigInteger* nonce, int pos) noexcept {
	BigInteger val = nonce->shiftRight(pos);
	RandomShaker shaker(&val, 4);

	this->arg01 = shaker.get(0);
	this->arg02 = shaker.get(1);
	this->arg03 = shaker.get(2);
	this->out01 = shaker.get(3);
}

void AbstractHashAlgorithmElement::execute(char **params) {
	calulate(params[this->arg01], params[this->arg02], params[this->arg03], params[this->out01]);
}

} /* namespace codablecash */
