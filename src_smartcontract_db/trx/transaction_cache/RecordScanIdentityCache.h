/*
 * RecordScanIdentityCache.h
 *
 *  Created on: 2022/01/27
 *      Author: iizuka
 */

#ifndef TRX_TRANSACTION_CACHE_RECORDSCANIDENTITYCACHE_H_
#define TRX_TRANSACTION_CACHE_RECORDSCANIDENTITYCACHE_H_

#include "trx/transaction_cache/AbstractSwapCache.h"

namespace codablecash {

class CdbKeyFactory;
class CdbDataFactory;
class ScanRecordIndentity;

class RecordScanIdentityCache : public AbstractSwapCache {
public:
	RecordScanIdentityCache(const File* folder, const UnicodeString* name, CdbKeyFactory* keyFactory, CdbDataFactory* dataFactory, DiskCacheManager* diskCache);
	virtual ~RecordScanIdentityCache();

	void insert(const ScanRecordIndentity* identity);
	bool hasKey(const ScanRecordIndentity* identity);
};

} /* namespace codablecash */

#endif /* TRX_TRANSACTION_CACHE_RECORDSCANIDENTITYCACHE_H_ */
