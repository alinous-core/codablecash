/*
 * BlockMerkleRootKeyFactory.h
 *
 *  Created on: 2023/03/02
 *      Author: iizuka
 */

#ifndef BC_BLOCK_BLOCKMERKLEROOTKEYFACTORY_H_
#define BC_BLOCK_BLOCKMERKLEROOTKEYFACTORY_H_

#include "btreekey/BtreeKeyFactory.h"

using namespace alinous;

namespace codablecash {

class BlockMerkleRootKeyFactory : public BtreeKeyFactory {
public:
	static const constexpr uint32_t MERKLE_ROOT_KEY{0x15};

	BlockMerkleRootKeyFactory();
	virtual ~BlockMerkleRootKeyFactory();

	virtual AbstractBtreeKey* fromBinary(uint32_t keyType, ByteBuffer* in) const;
	virtual BtreeKeyFactory* copy() const noexcept;
};

} /* namespace codablecash */

#endif /* BC_BLOCK_BLOCKMERKLEROOTKEYFACTORY_H_ */
