/*
 * BlockFileStore.h
 *
 *  Created on: 2018/10/06
 *      Author: iizuka
 */

#ifndef FILESTORE_BLOCK_BLOCKFILESTORE_H_
#define FILESTORE_BLOCK_BLOCKFILESTORE_H_

#include "filestore/FileStore.h"
#include "filestore_block/IBlockFileStore.h"

namespace alinous {
	class BlockFileHeader;
	class BlockFileBody;
	class BlockHandle;
}

namespace alinous {

class IBlockHandle;

class BlockFileStore : public FileStore, public IBlockFileStore {
public:
	BlockFileStore(const FileStore& base) = delete;
	BlockFileStore(const UnicodeString* dir, const UnicodeString* name, DiskCacheManager* cacheManager) noexcept;
	virtual ~BlockFileStore() noexcept;


	void createStore(bool del, uint64_t defaultSize) noexcept(false);
	virtual void createStore(bool del, uint64_t defaultSize, uint64_t blockSize) noexcept(false);

	virtual void open(bool sync) noexcept(false);
	bool isOpened() const noexcept;
	void close() noexcept;

	virtual IBlockHandle* alloc(uint64_t size);
	virtual IBlockHandle* get(uint64_t fpos);
	virtual void sync(bool fsync);

	BlockFileBody* getBody() const {
		return body;
	}
	BlockFileHeader* getHeader() const {
		return header;
	}

private:
	void internalClear() noexcept;
	void internalAllocBody(BlockHandle* handle, const uint64_t size);

private:
	BlockFileHeader* header;
	BlockFileBody* body;
};

} /* namespace alinous */

#endif /* FILESTORE_BLOCK_BLOCKFILESTORE_H_ */
