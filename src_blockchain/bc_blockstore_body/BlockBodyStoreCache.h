/*
 * BlockBodyStoreCache.h
 *
 *  Created on: 2023/03/01
 *      Author: iizuka
 */

#ifndef BC_BLOCKSTORE_BODY_BLOCKBODYSTORECACHE_H_
#define BC_BLOCKSTORE_BODY_BLOCKBODYSTORECACHE_H_

#include <cstdint>

#include "base/RawLinkedList.h"

namespace alinous {
class File;
}
using namespace alinous;

namespace codablecash {

class BlockBodyStoreCacheElement;

class BlockBodyStoreCache {
public:
	BlockBodyStoreCache(const File* baseDir, int sectionLimit);
	virtual ~BlockBodyStoreCache();

	BlockBodyStoreCacheElement* getBodyStoreCacheElement(uint64_t height, bool create);

private:
	BlockBodyStoreCacheElement* findCache(uint64_t index) noexcept;

private:
	const File* baseDir;
	int sectionLimit;
	int maxCache;
	RawLinkedList<BlockBodyStoreCacheElement>* list;
};

} /* namespace codablecash */

#endif /* BC_BLOCKSTORE_BODY_BLOCKBODYSTORECACHE_H_ */
