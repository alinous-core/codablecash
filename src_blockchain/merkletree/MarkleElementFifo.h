/*
 * MarkleElementStack.h
 *
 *  Created on: 2022/03/22
 *      Author: iizuka
 */

#ifndef MERKLETREE_MARKLEELEMENTFIFO_H_
#define MERKLETREE_MARKLEELEMENTFIFO_H_

#include "base/ArrayList.h"

using namespace alinous;

namespace codablecash {

class AbstractMerkleElement;

class MarkleElementFifo {
public:
	MarkleElementFifo();
	virtual ~MarkleElementFifo();

	void addElement(AbstractMerkleElement* element) noexcept;
	AbstractMerkleElement* out() noexcept;

	int size() const noexcept;
	bool isEmpty() const noexcept;

private:
	ArrayList<AbstractMerkleElement> list;
};

} /* namespace codablecash */

#endif /* MERKLETREE_MARKLEELEMENTFIFO_H_ */
