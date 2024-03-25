/*
 * BlockMerkleRootKey.h
 *
 *  Created on: 2023/03/02
 *      Author: iizuka
 */

#ifndef BC_BLOCK_BLOCKMERKLEROOTKEY_H_
#define BC_BLOCK_BLOCKMERKLEROOTKEY_H_

#include "btree/AbstractBtreeKey.h"

using namespace alinous;

namespace codablecash {

class BlockMerkleRoot;

class BlockMerkleRootKey : public AbstractBtreeKey {
public:
	explicit BlockMerkleRootKey(const BlockMerkleRoot* root);
	virtual ~BlockMerkleRootKey();

	virtual bool isInfinity() const { return false; }
	virtual bool isNull() const { return false; }

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	static BlockMerkleRootKey* fromBinary(ByteBuffer* in);

	virtual int compareTo(const AbstractBtreeKey* key) const noexcept;
	virtual AbstractBtreeKey* clone() const noexcept;

private:
	BlockMerkleRoot* root;
};

} /* namespace codablecash */

#endif /* BC_BLOCK_BLOCKMERKLEROOTKEY_H_ */
