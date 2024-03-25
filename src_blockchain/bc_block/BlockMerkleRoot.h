/*
 * BlockMerkleRoot.h
 *
 *  Created on: 2023/03/02
 *      Author: iizuka
 */

#ifndef BC_BLOCK_BLOCKMERKLEROOT_H_
#define BC_BLOCK_BLOCKMERKLEROOT_H_

#include "bc_base/Abstract32BytesId.h"

namespace codablecash {

class BlockMerkleRoot : public Abstract32BytesId {
public:
	BlockMerkleRoot(const char* binary, int length);
	virtual ~BlockMerkleRoot();

	static BlockMerkleRoot* createZeroRoot();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	static BlockMerkleRoot* fromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;
};

} /* namespace codablecash */

#endif /* BC_BLOCK_BLOCKMERKLEROOT_H_ */
