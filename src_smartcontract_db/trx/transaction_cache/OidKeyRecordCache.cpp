/*
 * OidKeyRecordCache.cpp
 *
 *  Created on: 2020/09/29
 *      Author: iizuka
 */

#include "trx/transaction_cache/OidKeyRecordCache.h"

#include "trx/transaction_cache/OidKeyRecordCacheScanner.h"

#include "base/UnicodeString.h"
#include "base/StackRelease.h"

#include "schema_table/record/table_record/CdbKeyFactory.h"
#include "schema_table/record/table_record/CdbDataFactory.h"
#include "schema_table/record/table_record/CdbRecord.h"

#include "schema_table/record/table_record_key/CdbOidKey.h"

#include "random_access_file/DiskCacheManager.h"

#include "engine/CdbOid.h"
#include "engine/CdbException.h"


namespace codablecash {

OidKeyRecordCache::OidKeyRecordCache(const File* folder, const UnicodeString* name, CdbKeyFactory* keyFactory, CdbDataFactory* dataFactory, DiskCacheManager* diskCache)
			: AbstractSwapCache(folder, name, keyFactory, dataFactory, diskCache) {
}

OidKeyRecordCache::~OidKeyRecordCache() {

}

void OidKeyRecordCache::insert(const CdbRecord* value) {
	const CdbOid* oid = value->getOid();
	AbstractCdbKey* key = oid->toKey(); __STP(key);

	if(hasKey(dynamic_cast<const CdbOidKey*>(key))){
		throw new CdbException(L"Key is already stored in cache.", __FILE__, __LINE__);
	}

	int diff = value->binarySize();
	putData(key, value);

	this->currentSize += diff;
	swapIfNecessary();
}

void OidKeyRecordCache::remove(const CdbOid* oid) {
	AbstractCdbKey* key = oid->toKey(); __STP(key);
	removeData(key);
}

bool OidKeyRecordCache::hasKey(const CdbOidKey* key) {
	const IBlockObject* data = findData(key);

	return data != nullptr;
}

const CdbRecord* OidKeyRecordCache::findRecord(const CdbOidKey* key) {
	const IBlockObject* data = findData(key);
	const CdbRecord* record = dynamic_cast<const CdbRecord*>(data);

	return record;
}

OidKeyRecordCacheScanner* OidKeyRecordCache::getRecordScanner() {
	OidKeyRecordCacheScanner* s = new OidKeyRecordCacheScanner(this);
	s->init();

	return s;
}

} /* namespace codablecash */
