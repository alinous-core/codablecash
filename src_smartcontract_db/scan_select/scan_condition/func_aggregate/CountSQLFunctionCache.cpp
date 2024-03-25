/*
 * CountSQLFunctionCache.cpp
 *
 *  Created on: 2021/11/03
 *      Author: iizuka
 */

#include "scan_select/scan_condition/func_aggregate/CountSQLFunctionCache.h"

#include "schema_table/record/table_record/CdbKeyFactory.h"
#include "schema_table/record/table_record/CdbDataFactory.h"

#include "schema_table/record/table_record_value/CdbLongValue.h"

#include "schema_table/record/table_record_key/AbstractCdbKey.h"

#include "base/StackRelease.h"


namespace codablecash {

CountSQLFunctionCache::CountSQLFunctionCache(const File* folder, const UnicodeString* name, CdbKeyFactory* keyFactory, CdbDataFactory* dataFactory, DiskCacheManager* diskCache)
			: AbstractSwapCache(folder, name, keyFactory, dataFactory, diskCache){
	this->indexSerial = 0;
}

CountSQLFunctionCache::~CountSQLFunctionCache() {

}

void CountSQLFunctionCache::addValue(const AbstractCdbKey* key) {
	uint64_t v = this->indexSerial++;
	CdbLongValue* longValue = new CdbLongValue(v); __STP(longValue);

	putData(key, longValue);

}

bool CountSQLFunctionCache::hasValue(const AbstractCdbKey* key) {
	const IBlockObject* obj = findData(key);

	return obj != nullptr;
}

} /* namespace codablecash */
