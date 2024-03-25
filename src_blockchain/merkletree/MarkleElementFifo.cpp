/*
 * MarkleElementStack.cpp
 *
 *  Created on: 2022/03/22
 *      Author: iizuka
 */

#include "merkletree/MarkleElementFifo.h"
#include "merkletree/AbstractMerkleElement.h"

namespace codablecash {

MarkleElementFifo::MarkleElementFifo() {

}

MarkleElementFifo::~MarkleElementFifo() {
	this->list.deleteElements();
}

void MarkleElementFifo::addElement(AbstractMerkleElement* element) noexcept {
	this->list.addElement(element);
}

AbstractMerkleElement* MarkleElementFifo::out() noexcept {
	return this->list.remove(0);
}

int MarkleElementFifo::size() const noexcept {
	return this->list.size();
}

bool MarkleElementFifo::isEmpty() const noexcept {
	return size() == 0;
}

} /* namespace codablecash */
