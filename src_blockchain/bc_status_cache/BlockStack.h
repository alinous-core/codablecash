/*
 * BlockStack.h
 *
 *  Created on: 2023/05/22
 *      Author: iizuka
 */

#ifndef BC_STATUS_CACHE_BLOCKSTACK_H_
#define BC_STATUS_CACHE_BLOCKSTACK_H_

#include "base/ArrayList.h"

using namespace alinous;

namespace codablecash {

class BlockStackElement;
class BlockHead;

class BlockStack {
public:
	BlockStack();
	virtual ~BlockStack();

	void push(BlockStackElement* element) noexcept;

	BlockStackElement* top() const noexcept;
	bool isEmpty() const noexcept;

	BlockHead* createBlockHead() const noexcept;

	void gotoBranch() noexcept;

private:
	ArrayList<BlockStackElement> list;
};

} /* namespace codablecash */

#endif /* BC_STATUS_CACHE_BLOCKSTACK_H_ */
