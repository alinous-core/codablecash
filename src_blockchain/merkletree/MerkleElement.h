/*
 * MarkleElement.h
 *
 *  Created on: 2022/03/21
 *      Author: iizuka
 */

#ifndef MERKLETREE_MERKLEELEMENT_H_
#define MERKLETREE_MERKLEELEMENT_H_

#include "merkletree/AbstractMerkleElement.h"

namespace codablecash {

class MerkleElement : public AbstractMerkleElement {
public:
	MerkleElement();
	virtual ~MerkleElement();

	void calcHash() noexcept;

	virtual const AbstractMerkleElement* find(const ByteBuffer* hash) const noexcept;
};

} /* namespace codablecash */

#endif /* MERKLETREE_MERKLEELEMENT_H_ */
