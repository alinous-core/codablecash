/*
 * BlockHeaderIdKey.h
 *
 *  Created on: 2023/05/01
 *      Author: iizuka
 */

#ifndef BC_BLOCKSTORE_HEADER_BLOCKHEADERIDKEY_H_
#define BC_BLOCKSTORE_HEADER_BLOCKHEADERIDKEY_H_

#include "btree/AbstractBtreeKey.h"

using namespace alinous;

namespace codablecash {

class BlockHeaderId;

class BlockHeaderIdKey : public AbstractBtreeKey {
public:
	explicit BlockHeaderIdKey(const BlockHeaderId* headerId);
	virtual ~BlockHeaderIdKey();

	virtual bool isInfinity() const { return false; }
	virtual bool isNull() const { return false; }

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	static BlockHeaderIdKey* fromBinary(ByteBuffer* in);

	virtual int compareTo(const AbstractBtreeKey* key) const noexcept;
	virtual AbstractBtreeKey* clone() const noexcept;

private:
	BlockHeaderId* headerId;
};

} /* namespace codablecash */

#endif /* BC_BLOCKSTORE_HEADER_BLOCKHEADERIDKEY_H_ */
