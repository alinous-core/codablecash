/*
 * HeadBlockDetectorCache.h
 *
 *  Created on: 2023/05/25
 *      Author: iizuka
 */

#ifndef BC_STATUS_CACHE_HEADBLOCKDETECTORCACHE_H_
#define BC_STATUS_CACHE_HEADBLOCKDETECTORCACHE_H_

#include "base/HashMap.h"


using namespace alinous;

namespace codablecash {

class BlockHeaderId;
class HeadBlockDetectorCacheElement;

class HeadBlockDetectorCache {
public:
	HeadBlockDetectorCache();
	virtual ~HeadBlockDetectorCache();

	void registerCache(const BlockHeaderId* headerId, HeadBlockDetectorCacheElement* cache) noexcept;
	const HeadBlockDetectorCacheElement* getCache(const BlockHeaderId* headerId) const noexcept;

	void reset() noexcept;

private:
	HashMap<BlockHeaderId, HeadBlockDetectorCacheElement> map;
};

} /* namespace codablecash */

#endif /* BC_STATUS_CACHE_HEADBLOCKDETECTORCACHE_H_ */
