/*
 * GroupKeyCache.h
 *
 *  Created on: 2021/05/20
 *      Author: iizuka
 */

#ifndef TRX_TRANSACTION_CACHE_GROUPCACHE_H_
#define TRX_TRANSACTION_CACHE_GROUPCACHE_H_

namespace codablecash {

class CdbLocalCacheManager;

class GroupCache {
public:
	explicit GroupCache(CdbLocalCacheManager* localCacheManager);
	virtual ~GroupCache();

private:
	CdbLocalCacheManager* localCacheManager;

};

} /* namespace codablecash */

#endif /* TRX_TRANSACTION_CACHE_GROUPCACHE_H_ */
