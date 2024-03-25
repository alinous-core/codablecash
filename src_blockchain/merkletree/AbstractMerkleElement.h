/*
 * AbstractMerkleElement.h
 *
 *  Created on: 2022/03/21
 *      Author: iizuka
 */

#ifndef MERKLETREE_ABSTRACTMERKLEELEMENT_H_
#define MERKLETREE_ABSTRACTMERKLEELEMENT_H_

#include "base/ArrayList.h"

namespace alinous {
class ByteBuffer;
}
using namespace alinous;

namespace codablecash {

class AbstractMerkleElement {
public:
	AbstractMerkleElement();
	virtual ~AbstractMerkleElement();

	int hashSize() const noexcept;

	void setHash(const ByteBuffer* b) noexcept;
	const ByteBuffer* getHash() const noexcept {
		return this->hash;
	}

	const AbstractMerkleElement* getAnotherPair() const noexcept;

	void addChild(AbstractMerkleElement* child) noexcept;
	void setParent(AbstractMerkleElement* parent) noexcept {
		this->parent = parent;
	}
	const AbstractMerkleElement* getParent() const noexcept {
		return this->parent;
	}

	bool isLeaf() const noexcept;
	bool isRoot() const noexcept;

	bool isLeft() const noexcept;

	virtual const AbstractMerkleElement* find(const ByteBuffer* hash) const noexcept = 0;

protected:
	ByteBuffer* hash;

	AbstractMerkleElement* parent;
	ArrayList<AbstractMerkleElement>* children;
};

} /* namespace codablecash */

#endif /* MERKLETREE_ABSTRACTMERKLEELEMENT_H_ */
