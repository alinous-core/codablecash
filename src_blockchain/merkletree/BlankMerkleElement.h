/*
 * BlankMerkleElement.h
 *
 *  Created on: 2022/03/21
 *      Author: iizuka
 */

#ifndef MERKLETREE_BLANKMERKLEELEMENT_H_
#define MERKLETREE_BLANKMERKLEELEMENT_H_

#include "merkletree/AbstractMerkleElement.h"

namespace codablecash {

class BlankMerkleElement : public AbstractMerkleElement {
public:
	static const UnicodeString BLANK_HASH;

	BlankMerkleElement();
	virtual ~BlankMerkleElement();

	virtual const AbstractMerkleElement* find(const ByteBuffer* hash) const noexcept;
};

} /* namespace codablecash */

#endif /* MERKLETREE_BLANKMERKLEELEMENT_H_ */
