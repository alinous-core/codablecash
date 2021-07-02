/*
 * GroupRecordCache.cpp
 *
 *  Created on: 2021/06/30
 *      Author: iizuka
 */

#include "trx/transaction_cache/GroupRecordCache.h"

#include "schema_table/record/table_record/CdbKeyFactory.h"
#include "schema_table/record/table_record/CdbDataFactory.h"
#include "schema_table/record/table_record/CdbRecord.h"

#include "schema_table/record/table_record_key/AbstractCdbKey.h"

#include "base/StackRelease.h"

#include "filestore_block/IBlockObject.h"

#include "schema_table/record/table_record_value/CdbLongValue.h"

namespace codablecash {

GroupRecordCache::GroupRecordCache(const File* folder, const UnicodeString* name, CdbKeyFactory* keyFactory, CdbDataFactory* dataFactory, DiskCacheManager* diskCache)
		: AbstractSwapCache(folder, name, keyFactory, dataFactory, diskCache) {
	this->indexSerial = 1;
}

GroupRecordCache::~GroupRecordCache() {

}

uint64_t GroupRecordCache::getRecordIndex(const CdbRecord* groupedRecord) {
	AbstractCdbKey* key = groupedRecord->toKey(); __STP(key);

	const IBlockObject* obj = findData(key);
	if(obj == nullptr){
		return newRecordIndex(key);
	}

	const CdbLongValue* longValue = dynamic_cast<const CdbLongValue*>(obj);
	uint64_t ret = longValue->getValue();

	return ret;
}

uint64_t GroupRecordCache::newRecordIndex(const AbstractCdbKey* key) {
	uint64_t v = this->indexSerial++;
	CdbLongValue* longValue = new CdbLongValue(v); __STP(longValue);

	putData(key, longValue);

	return v;
}

} /* namespace codablecash */
