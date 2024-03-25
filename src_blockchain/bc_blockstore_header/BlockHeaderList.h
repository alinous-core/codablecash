/*
 * BlockHeaderList.h
 *
 *  Created on: 2023/02/16
 *      Author: iizuka
 */

#ifndef BC_BLOCKSTORE_HEADER_BLOCKHEADERLIST_H_
#define BC_BLOCKSTORE_HEADER_BLOCKHEADERLIST_H_

#include "filestore_block/IBlockObject.h"

#include "base/RawArrayPrimitive.h"

using namespace alinous;

namespace codablecash {

class BlockHeaderList : public IBlockObject {
public:
	BlockHeaderList(const BlockHeaderList& inst);
	BlockHeaderList();
	virtual ~BlockHeaderList();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	static BlockHeaderList* fromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

	void join(const BlockHeaderList* value) noexcept;
	bool contains(uint64_t value) const noexcept;
	void remove(uint64_t value) noexcept;
	int indexof(uint64_t value) const noexcept;

	bool isEmpty() const noexcept;

	void add(uint64_t value) noexcept;

	int size() const noexcept;
	uint64_t get(int pos) const noexcept;

private:
	RawArrayPrimitive<uint64_t> list; // list of fpos
};

} /* namespace codablecash */

#endif /* BC_BLOCKSTORE_HEADER_BLOCKHEADERLIST_H_ */
