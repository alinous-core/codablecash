/*
 * BlockStack.cpp
 *
 *  Created on: 2023/05/22
 *      Author: iizuka
 */

#include "bc_status_cache/BlockStack.h"
#include "bc_status_cache/BlockStackElement.h"

#include "bc_status_cache/BlockHead.h"
namespace codablecash {

BlockStack::BlockStack() {

}

BlockStack::~BlockStack() {
	this->list.deleteElements();
}

void BlockStack::push(BlockStackElement *element) noexcept {
	this->list.addElement(element);
}

BlockStackElement* BlockStack::top() const noexcept {
	int pos = this->list.size() - 1;
	return this->list.get(pos);
}

bool BlockStack::isEmpty() const noexcept {
	return this->list.isEmpty();
}

BlockHead* BlockStack::createBlockHead() const noexcept {
	BlockHead* head = new BlockHead();

	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		BlockStackElement* element = this->list.get(i);
		const BlockHeader* header = element->current();

		head->add(header);
	}

	return head;
}

void BlockStack::gotoBranch() noexcept {
	BlockStackElement* element = top();
	while(!element->hasNext()){
		delete this->list.pop();
		if(isEmpty()){
			break;
		}
		element = top();
	}

	if(!isEmpty()){
		element = top();

		assert(element->hasNext());

		element->next();
	}
}

} /* namespace codablecash */
