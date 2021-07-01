/*
 * GroupRecordCache.h
 *
 *  Created on: 2021/06/30
 *      Author: iizuka
 */

#ifndef TRX_TRANSACTION_CACHE_GROUPRECORDCACHE_H_
#define TRX_TRANSACTION_CACHE_GROUPRECORDCACHE_H_

#include "trx/transaction_cache/AbstractSwapCache.h"

namespace codablecash {

class CdbKeyFactory;
class CdbDataFactory;
class CdbOidKey;
class CdbRecord;

class GroupRecordCache : public AbstractSwapCache {
public:
	GroupRecordCache(const File* folder, const UnicodeString* name, CdbKeyFactory* keyFactory, CdbDataFactory* dataFactory, DiskCacheManager* diskCache);
	virtual ~GroupRecordCache();


};

} /* namespace codablecash */

#endif /* TRX_TRANSACTION_CACHE_GROUPRECORDCACHE_H_ */
