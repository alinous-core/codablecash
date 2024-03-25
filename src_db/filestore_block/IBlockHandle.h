/*
 * IBlockHandle.h
 *
 *  Created on: 2023/02/08
 *      Author: iizuka
 */

#ifndef FILESTORE_BLOCK_IBLOCKHANDLE_H_
#define FILESTORE_BLOCK_IBLOCKHANDLE_H_

#include <cstdint>

namespace alinous {

class ByteBuffer;

class IBlockHandle {
public:
	IBlockHandle();
	virtual ~IBlockHandle();

	virtual uint64_t getFpos() const = 0;
	virtual void write(const char* bytes, int length) = 0;
	virtual ByteBuffer* getBuffer() const = 0;
	virtual void removeBlocks() = 0;
	virtual int size() const noexcept = 0;
};

} /* namespace alinous */

#endif /* FILESTORE_BLOCK_IBLOCKHANDLE_H_ */
