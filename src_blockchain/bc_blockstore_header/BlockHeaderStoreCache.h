/*
 * BlockHeaderStoreCache.h
 *
 *  Created on: 2023/02/16
 *      Author: iizuka
 */

#ifndef BC_BLOCKSTORE_HEADER_BLOCKHEADERSTORECACHE_H_
#define BC_BLOCKSTORE_HEADER_BLOCKHEADERSTORECACHE_H_

#include <cstdint>

#include "base/RawLinkedList.h"

namespace alinous {
class File;
}
using namespace alinous;

namespace codablecash {

class BlockHeaderStoreCacheElement;

class BlockHeaderStoreCache {
public:
	BlockHeaderStoreCache(const File* baseDir, int sectionLimit);
	virtual ~BlockHeaderStoreCache();

	BlockHeaderStoreCacheElement* getHeaderStoreCacheElement(uint64_t height, bool create);

private:
	BlockHeaderStoreCacheElement* findCache(uint64_t index) noexcept;

private:
	const File* baseDir;
	int sectionLimit;
	int maxCache;
	RawLinkedList<BlockHeaderStoreCacheElement>* list;
};

} /* namespace codablecash */

#endif /* BC_BLOCKSTORE_HEADER_BLOCKHEADERSTORECACHE_H_ */
