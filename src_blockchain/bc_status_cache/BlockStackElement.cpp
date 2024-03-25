/*
 * BlockStackElement.cpp
 *
 *  Created on: 2023/05/22
 *      Author: iizuka
 */

#include "bc_status_cache/BlockStackElement.h"

#include "bc_block/BlockHeader.h"

namespace codablecash {

BlockStackElement::BlockStackElement() {
	this->pos = 0;
}

BlockStackElement::~BlockStackElement() {
	this->list.deleteElements();
}

void BlockStackElement::addHeader(const BlockHeader *header) noexcept {
	this->list.addElement(dynamic_cast<BlockHeader*>(header->copyData()));
}

const BlockHeader* BlockStackElement::current() const noexcept {
	return this->list.get(this->pos);
}

bool BlockStackElement::hasNext() const noexcept {
	int maxIndex = this->list.size() - 1;
	return maxIndex > this->pos;
}

void BlockStackElement::next() noexcept {
	this->pos++;
}

} /* namespace codablecash */
