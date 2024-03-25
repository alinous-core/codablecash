/*
 * BlockHeaderHeightIndexKeyFactory.h
 *
 *  Created on: 2023/02/16
 *      Author: iizuka
 */

#ifndef BC_BLOCKSTORE_HEADER_BLOCKHEADERHEIGHTINDEXKEYFACTORY_H_
#define BC_BLOCKSTORE_HEADER_BLOCKHEADERHEIGHTINDEXKEYFACTORY_H_

#include "btreekey/BtreeKeyFactory.h"

using namespace alinous;

namespace codablecash {

class BlockHeaderHeightIndexKeyFactory : public BtreeKeyFactory {
public:
	static const constexpr uint32_t HEIGHT_INDEX_KEY{0x14};

	BlockHeaderHeightIndexKeyFactory();
	virtual ~BlockHeaderHeightIndexKeyFactory();

	virtual AbstractBtreeKey* fromBinary(uint32_t keyType, ByteBuffer* in) const;
	virtual BtreeKeyFactory* copy() const noexcept;
};

} /* namespace codablecash */

#endif /* BC_BLOCKSTORE_HEADER_BLOCKHEADERHEIGHTINDEXKEYFACTORY_H_ */
