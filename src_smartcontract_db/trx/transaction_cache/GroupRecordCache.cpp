/*
 * GroupRecordCache.cpp
 *
 *  Created on: 2021/06/30
 *      Author: iizuka
 */

#include "trx/transaction_cache/GroupRecordCache.h"

#include "schema_table/record/table_record/CdbKeyFactory.h"
#include "schema_table/record/table_record/CdbDataFactory.h"

namespace codablecash {

GroupRecordCache::GroupRecordCache(const File* folder, const UnicodeString* name, CdbKeyFactory* keyFactory, CdbDataFactory* dataFactory, DiskCacheManager* diskCache)
		: AbstractSwapCache(folder, name, keyFactory, dataFactory, diskCache) {

}

GroupRecordCache::~GroupRecordCache() {

}

} /* namespace codablecash */
