/*
 * BlockHeaderHeightIndex.h
 *
 *  Created on: 2023/02/16
 *      Author: iizuka
 */

#ifndef BC_BLOCKSTORE_HEADER_BLOCKHEADERHEIGHTINDEX_H_
#define BC_BLOCKSTORE_HEADER_BLOCKHEADERHEIGHTINDEX_H_
#include <cstdint>

namespace alinous {
class File;
class DiskCacheManager;
class Btree;
class UnicodeString;
}
using namespace alinous;

namespace codablecash {

class BlockHeaderList;

class BlockHeaderHeightIndex {
public:
	static const constexpr wchar_t* FILE_NAME{L"height_idx"};

	BlockHeaderHeightIndex(int index, const File* baseDir, DiskCacheManager* cacheManager);
	virtual ~BlockHeaderHeightIndex();

	bool exists() const noexcept;
	void create();

	void open();
	void close() noexcept;

	void addHeader(uint64_t height, uint64_t fpos);
	BlockHeaderList* getHeadersAtHeight(uint64_t height);
	void removeIndex(uint64_t height, uint64_t fpos);

private:
	void addIdex2String(UnicodeString* str) const noexcept;

private:
	int index;
	File* baseDir;
	DiskCacheManager* cacheManager;
	Btree* btree;
};

} /* namespace codablecash */

#endif /* BC_BLOCKSTORE_HEADER_BLOCKHEADERHEIGHTINDEX_H_ */
