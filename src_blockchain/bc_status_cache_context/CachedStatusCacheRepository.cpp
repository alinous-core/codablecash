/*
 * CachedStatusCacheRepository.cpp
 *
 *  Created on: 2023/04/26
 *      Author: iizuka
 */

#include "bc_status_cache_context/CachedStatusCacheRepository.h"
#include "bc_status_cache_context/CachedStatusCache.h"

#include "base/StackRelease.h"

#include "base_io/File.h"

#include "bc_block/BlockHeaderId.h"

namespace codablecash {

CachedStatusCacheRepository::CachedStatusCacheRepository(const File* tmpCacheBaseDir) {
	this->serial = 0;

	this->baseDir = tmpCacheBaseDir->get(CACHE_REPOSITORY_PATH);

	this->baseDir->deleteDir();
	this->baseDir->mkdirs();

	this->map = new HashMap<BlockHeaderId, CachedStatusCache>();
}

CachedStatusCacheRepository::~CachedStatusCacheRepository() {
	this->baseDir->deleteDir();
	delete this->baseDir;

	{
		Iterator<BlockHeaderId>* it = this->map->keySet()->iterator(); __STP(it);
		while(it->hasNext()){
			const BlockHeaderId* key = it->next();
			CachedStatusCache* cache = this->map->get(key);

			delete cache;
		}
	}
	delete this->map;
}

void CachedStatusCacheRepository::registerCache(uint64_t height, const BlockHeaderId *headerId, StatusCacheContext *context) {
	CachedStatusCache* c = getCache(headerId);

	if(c == nullptr){
		CachedStatusCache* newCache = new CachedStatusCache(++this->serial, this->baseDir, height); __STP(newCache);
		this->map->put(headerId, __STP_MV(newCache));

		newCache->init();
		newCache->importStatusContext(context);
	}
}

CachedStatusCache* CachedStatusCacheRepository::getCache(const BlockHeaderId *headerId) const noexcept {
	return this->map->get(headerId);
}

void CachedStatusCacheRepository::clear() {
	{
		Iterator<BlockHeaderId>* it = this->map->keySet()->iterator(); __STP(it);
		while(it->hasNext()){
			const BlockHeaderId* key = it->next();
			CachedStatusCache* cache = this->map->get(key);

			delete cache;
		}

		this->map->clear();
	}

	this->baseDir->deleteDir();
	this->baseDir->mkdirs();
}

} /* namespace codablecash */
