/*
 * AbstractContainerHashAlgorithmElement.cpp
 *
 *  Created on: 2023/03/09
 *      Author: iizuka
 */

#include "pow_hash/AbstractContainerHashAlgorithmElement.h"

namespace codablecash {

AbstractContainerHashAlgorithmElement::AbstractContainerHashAlgorithmElement()
		: AbstractHashAlgorithmElement() {
	this->list = new ArrayList<AbstractHashAlgorithmElement>();
}

AbstractContainerHashAlgorithmElement::~AbstractContainerHashAlgorithmElement() {
	this->list->deleteElements();
	delete this->list;
}

void AbstractContainerHashAlgorithmElement::addOperand(AbstractHashAlgorithmElement *ope) noexcept {
	this->list->addElement(ope);
}

void AbstractContainerHashAlgorithmElement::buildInputs(const BigInteger *nonce, int pos) noexcept {
	AbstractHashAlgorithmElement::buildInputs(nonce, pos);

	int maxLoop = this->list->size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractHashAlgorithmElement* algo = this->list->get(i);
		algo->buildInputs(nonce, i);
	}
}

} /* namespace codablecash */
