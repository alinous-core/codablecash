/*
 * GroupKeyCache.h
 *
 *  Created on: 2021/05/20
 *      Author: iizuka
 */

#ifndef TRX_TRANSACTION_CACHE_GROUPKEYCACHE_H_
#define TRX_TRANSACTION_CACHE_GROUPKEYCACHE_H_

namespace codablecash {

class CdbLocalCacheManager;

class GroupKeyCache {
public:
	explicit GroupKeyCache(CdbLocalCacheManager* localCacheManager);
	virtual ~GroupKeyCache();

private:
	CdbLocalCacheManager* localCacheManager;

};

} /* namespace codablecash */

#endif /* TRX_TRANSACTION_CACHE_GROUPKEYCACHE_H_ */
