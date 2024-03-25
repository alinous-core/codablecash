/*
 * IBlockFileStore.h
 *
 *  Created on: 2023/02/08
 *      Author: iizuka
 */

#ifndef FILESTORE_BLOCK_IBLOCKFILESTORE_H_
#define FILESTORE_BLOCK_IBLOCKFILESTORE_H_

#include <cstdint>

namespace alinous {

class IBlockHandle;

class IBlockFileStore {
public:
	IBlockFileStore();
	virtual ~IBlockFileStore();

	virtual void createStore(bool del, uint64_t defaultSize, uint64_t blockSize) noexcept(false) = 0;

	virtual void open(bool sync) noexcept(false) = 0;
	virtual void close() noexcept = 0;

	virtual IBlockHandle* alloc(uint64_t size) = 0;
	virtual IBlockHandle* get(uint64_t fpos) = 0;
	virtual void sync(bool fsync) = 0;
};

} /* namespace alinous */

#endif /* FILESTORE_BLOCK_IBLOCKFILESTORE_H_ */
