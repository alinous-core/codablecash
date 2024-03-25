/*
 * RecordScanIdentityCache.cpp
 *
 *  Created on: 2022/01/27
 *      Author: iizuka
 */

#include "trx/transaction_cache/RecordScanIdentityCache.h"

#include "random_access_file/DiskCacheManager.h"

#include "filestore_block/IBlockObject.h"

#include "schema_table/record/table_record/CdbDataFactory.h"
#include "schema_table/record/table_record/CdbKeyFactory.h"
#include "schema_table/record/table_record/ScanRecordIndentity.h"

#include "schema_table/record/table_record_key/AbstractCdbKey.h"

#include "base/StackRelease.h"


namespace codablecash {

RecordScanIdentityCache::RecordScanIdentityCache(const File* folder, const UnicodeString* name, CdbKeyFactory* keyFactory, CdbDataFactory* dataFactory, DiskCacheManager* diskCache)
			: AbstractSwapCache(folder, name, keyFactory, dataFactory, diskCache) {

}

RecordScanIdentityCache::~RecordScanIdentityCache() {

}

void RecordScanIdentityCache::insert(const ScanRecordIndentity* identity) {
	AbstractCdbKey* key = identity->toKey(); __STP(key);

	putData(key, identity);
}

bool RecordScanIdentityCache::hasKey(const ScanRecordIndentity* identity) {
	AbstractCdbKey* key = identity->toKey(); __STP(key);

	const IBlockObject* lastObj = findData(key);
	return lastObj != nullptr;
}

} /* namespace codablecash */
