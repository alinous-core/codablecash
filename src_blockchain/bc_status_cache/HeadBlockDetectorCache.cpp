/*
 * HeadBlockDetectorCache.cpp
 *
 *  Created on: 2023/05/25
 *      Author: iizuka
 */

#include "bc_status_cache/HeadBlockDetectorCache.h"
#include "bc_status_cache/HeadBlockDetectorCacheElement.h"

#include "bc_block/BlockHeaderId.h"

#include "base/StackRelease.h"


namespace codablecash {

HeadBlockDetectorCache::HeadBlockDetectorCache() {

}

HeadBlockDetectorCache::~HeadBlockDetectorCache() {
	reset();
}

void HeadBlockDetectorCache::registerCache(const BlockHeaderId *headerId, HeadBlockDetectorCacheElement *cache) noexcept {
	delete this->map.put(headerId, cache);
}

const HeadBlockDetectorCacheElement* HeadBlockDetectorCache::getCache(const BlockHeaderId *headerId) const noexcept {
	return this->map.get(headerId);
}

void HeadBlockDetectorCache::reset() noexcept {
	Iterator<BlockHeaderId>* it = this->map.keySet()->iterator(); __STP(it);
	while(it->hasNext()){
		const BlockHeaderId* key = it->next();
		HeadBlockDetectorCacheElement* v = this->map.get(key);

		delete v;
	}

	this->map.clear();
}

} /* namespace codablecash */
