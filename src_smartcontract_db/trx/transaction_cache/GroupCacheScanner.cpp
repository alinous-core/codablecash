/*
 * GroupCacheScanner.cpp
 *
 *  Created on: 2021/07/12
 *      Author: iizuka
 */

#include "trx/transaction_cache/GroupCacheScanner.h"
#include "trx/transaction_cache/GroupCache.h"
#include "trx/transaction_cache/GroupRecordCache.h"

#include "btree/IBtreeScanner.h"
#include "btree/AbstractBtreeKey.h"

#include "schema_table/record/table_record/CdbGroupedRecord.h"
#include "schema_table/record/table_record/CdbRecord.h"

#include "schema_table/record/table_record_key/CdbRecordKey.h"

#include "base/StackRelease.h"


namespace codablecash {

GroupCacheScanner::GroupCacheScanner(GroupCache* gcache) {
	this->gcache = gcache;
	this->scanner = nullptr;
	this->nextObj = nullptr;
}

GroupCacheScanner::~GroupCacheScanner() {
	this->gcache = nullptr;
	delete this->scanner;
	delete this->nextObj;
}

void GroupCacheScanner::init() {
	GroupRecordCache* grecordCache = this->gcache->getCache();

	this->scanner = grecordCache->getScanner();
	this->scanner->begin();
}

bool GroupCacheScanner::hasNext() {
	return this->scanner->hasNext();
}

const CdbGroupedRecord* GroupCacheScanner::next(OidKeyRecordCache* orgCache) {
	delete this->nextObj;
	this->nextObj = nullptr;

	const IBlockObject* obj = this->scanner->next();
	const AbstractBtreeKey* key = this->scanner->nextKey();

	const CdbRecordKey* recKey = dynamic_cast<const CdbRecordKey*>(key);

	AbstractCdbValue* recValue = recKey->toCdbValue(); __STP(recValue);
	CdbRecord* rec = dynamic_cast<CdbRecord*>(recValue);


	OidArrayCache* oidArrayCache = this->gcache->getArrayCache();
	this->nextObj = new CdbGroupedRecord(rec, this->gcache, oidArrayCache, orgCache);

	return this->nextObj;
}

} /* namespace codablecash */
