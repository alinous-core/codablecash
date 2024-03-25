/*
 * BlockHeaderIdKeyFactory.h
 *
 *  Created on: 2023/05/01
 *      Author: iizuka
 */

#ifndef BC_BLOCKSTORE_HEADER_BLOCKHEADERIDKEYFACTORY_H_
#define BC_BLOCKSTORE_HEADER_BLOCKHEADERIDKEYFACTORY_H_

#include "btreekey/BtreeKeyFactory.h"

using namespace alinous;

namespace codablecash {

class BlockHeaderIdKeyFactory : public BtreeKeyFactory {
public:
	static const constexpr uint32_t BLOCK_HEADER_ID_KEY{0x14};

	BlockHeaderIdKeyFactory();
	virtual ~BlockHeaderIdKeyFactory();

	virtual AbstractBtreeKey* fromBinary(uint32_t keyType, ByteBuffer* in) const;
	virtual BtreeKeyFactory* copy() const noexcept;
};

} /* namespace codablecash */

#endif /* BC_BLOCKSTORE_HEADER_BLOCKHEADERIDKEYFACTORY_H_ */
