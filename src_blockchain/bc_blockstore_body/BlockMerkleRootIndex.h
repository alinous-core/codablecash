/*
 * BlockMerkleRootIndex.h
 *
 *  Created on: 2023/03/02
 *      Author: iizuka
 */

#ifndef BC_BLOCKSTORE_BODY_BLOCKMERKLEROOTINDEX_H_
#define BC_BLOCKSTORE_BODY_BLOCKMERKLEROOTINDEX_H_
#include <cstdint>

namespace alinous {
class File;
class DiskCacheManager;
class Btree;
class UnicodeString;
}
using namespace alinous;


namespace codablecash {

class BlockMerkleRoot;

class BlockMerkleRootIndex {
public:
	static const constexpr wchar_t* FILE_NAME{L"merkle_root_idx"};

	BlockMerkleRootIndex(int index, const File* baseDir, DiskCacheManager* cacheManager);
	virtual ~BlockMerkleRootIndex();

	bool exists() const noexcept;
	void create();

	void open();
	void close() noexcept;

	void addBody(const BlockMerkleRoot* root, uint64_t fpos);
	uint64_t getBodyFpos(const BlockMerkleRoot* root);
	void removeBody(const BlockMerkleRoot* root);


private:
	void addIdex2String(UnicodeString* str) const noexcept;

private:
	int index;
	File* baseDir;
	DiskCacheManager* cacheManager;
	Btree* btree;
};

} /* namespace codablecash */

#endif /* BC_BLOCKSTORE_BODY_BLOCKMERKLEROOTINDEX_H_ */
