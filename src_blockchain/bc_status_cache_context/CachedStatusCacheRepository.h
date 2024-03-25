/*
 * CachedStatusCacheRepository.h
 *
 *  Created on: 2023/04/26
 *      Author: iizuka
 */

#ifndef BC_STATUS_CACHE_CONTEXT_CACHEDSTATUSCACHEREPOSITORY_H_
#define BC_STATUS_CACHE_CONTEXT_CACHEDSTATUSCACHEREPOSITORY_H_
#include <cstdint>

#include "base/HashMap.h"

namespace alinous {
class File;
}
using namespace alinous;

namespace codablecash {

class BlockHeaderId;
class StatusCacheContext;
class CachedStatusCache;

class CachedStatusCacheRepository {
public:
	static const constexpr wchar_t* CACHE_REPOSITORY_PATH{L"cache"};

	explicit CachedStatusCacheRepository(const File* tmpCacheBaseDir);
	virtual ~CachedStatusCacheRepository();

	void registerCache(uint64_t height, const BlockHeaderId *headerId, StatusCacheContext* context);
	CachedStatusCache* getCache(const BlockHeaderId *headerId) const noexcept;

	void clear();

private:
	File* baseDir;
	uint64_t serial;

	HashMap<BlockHeaderId, CachedStatusCache>* map;
};

} /* namespace codablecash */

#endif /* BC_STATUS_CACHE_CONTEXT_CACHEDSTATUSCACHEREPOSITORY_H_ */
