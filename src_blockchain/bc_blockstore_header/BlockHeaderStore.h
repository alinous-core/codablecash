/*
 * BlockHeaderStore.h
 *
 *  Created on: 2023/02/16
 *      Author: iizuka
 */

#ifndef BC_BLOCKSTORE_HEADER_BLOCKHEADERSTORE_H_
#define BC_BLOCKSTORE_HEADER_BLOCKHEADERSTORE_H_

#include <cstdint>

namespace alinous {
class File;
class DiskCacheManager;
class VariableBlockFileStore;
class UnicodeString;
}
using namespace alinous;

namespace codablecash {

class BlockHeader;

class BlockHeaderStore {
public:
	static const constexpr wchar_t* FILE_NAME{L"headerstore"};

	BlockHeaderStore(int index, const File* baseDir, DiskCacheManager* cacheManager);
	virtual ~BlockHeaderStore();

	bool exists() const noexcept;
	void create();

	void open();
	void close() noexcept;

	uint64_t storeHeader(const BlockHeader* header);
	BlockHeader* loadHeader(uint64_t fpos);
	void removeHeader(uint64_t fpos);

private:
	File getStoreFile() const noexcept;
	File getStoreHeaderFile() const noexcept;
	void addIdex2String(UnicodeString* str) const noexcept;

private:
	int index;
	File* baseDir;
	DiskCacheManager* cacheManager;
	VariableBlockFileStore* store;
};

} /* namespace codablecash */

#endif /* BC_BLOCKSTORE_HEADER_BLOCKHEADERSTORE_H_ */
