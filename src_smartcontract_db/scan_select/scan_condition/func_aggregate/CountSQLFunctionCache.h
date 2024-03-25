/*
 * CountSQLFunctionCache.h
 *
 *  Created on: 2021/11/03
 *      Author: iizuka
 */

#ifndef SCAN_SELECT_SCAN_CONDITION_FUNC_AGGREGATE_COUNTSQLFUNCTIONCACHE_H_
#define SCAN_SELECT_SCAN_CONDITION_FUNC_AGGREGATE_COUNTSQLFUNCTIONCACHE_H_

#include "trx/transaction_cache/AbstractSwapCache.h"

namespace codablecash {

class CdbKeyFactory;
class CdbDataFactory;
class CdbOidKey;
class CdbRecord;
class AbstractCdbKey;

class CountSQLFunctionCache : public AbstractSwapCache {
public:
	CountSQLFunctionCache(const File* folder, const UnicodeString* name, CdbKeyFactory* keyFactory, CdbDataFactory* dataFactory, DiskCacheManager* diskCache);
	virtual ~CountSQLFunctionCache();

	void addValue(const AbstractCdbKey* key);
	bool hasValue(const AbstractCdbKey* key);

private:
	uint64_t indexSerial;
};

} /* namespace codablecash */

#endif /* SCAN_SELECT_SCAN_CONDITION_FUNC_AGGREGATE_COUNTSQLFUNCTIONCACHE_H_ */
