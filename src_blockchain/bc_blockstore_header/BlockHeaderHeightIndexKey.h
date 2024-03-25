/*
 * BlockHeaderHeightIndexKey.h
 *
 *  Created on: 2023/02/16
 *      Author: iizuka
 */

#ifndef BC_BLOCKSTORE_HEADER_BLOCKHEADERHEIGHTINDEXKEY_H_
#define BC_BLOCKSTORE_HEADER_BLOCKHEADERHEIGHTINDEXKEY_H_

#include "btree/AbstractBtreeKey.h"

using namespace alinous;

namespace codablecash {

class BlockHeaderHeightIndexKey : public AbstractBtreeKey {
public:
	explicit BlockHeaderHeightIndexKey(uint64_t value);
	BlockHeaderHeightIndexKey(uint64_t value, uint64_t fpos);
	virtual ~BlockHeaderHeightIndexKey();

	virtual bool isInfinity() const { return false; }
	virtual bool isNull() const { return false; }

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	static BlockHeaderHeightIndexKey* fromBinary(ByteBuffer* in);

	virtual int compareTo(const AbstractBtreeKey* key) const noexcept;
	virtual AbstractBtreeKey* clone() const noexcept;

	uint64_t getFpos() const {
		return fpos;
	}

protected:
	uint64_t value;
	uint64_t fpos;
};

} /* namespace codablecash */

#endif /* BC_BLOCKSTORE_HEADER_BLOCKHEADERHEIGHTINDEXKEY_H_ */
