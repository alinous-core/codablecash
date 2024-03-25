/*
 * OidKeyRecordCacheScanner.cpp
 *
 *  Created on: 2022/01/09
 *      Author: iizuka
 */

#include "trx/transaction_cache/OidKeyRecordCacheScanner.h"
#include "trx/transaction_cache/OidKeyRecordCache.h"

#include "btree/IBtreeScanner.h"

#include "schema_table/record/table_record/CdbRecord.h"


namespace codablecash {

OidKeyRecordCacheScanner::OidKeyRecordCacheScanner(OidKeyRecordCache* cache) {
	this->cache = cache;
	this->scanner = nullptr;
}

OidKeyRecordCacheScanner::~OidKeyRecordCacheScanner() {
	this->cache = nullptr;
	delete this->scanner;
}

void OidKeyRecordCacheScanner::init() {
	this->scanner = this->cache->getScanner();
	this->scanner->begin();
}

bool OidKeyRecordCacheScanner::hasNext() {
	return this->scanner->hasNext();
}

CdbRecord* OidKeyRecordCacheScanner::next() {
	const IBlockObject* obj = this->scanner->next();

	IBlockObject* newObj = obj->copyData();
	CdbRecord* record = dynamic_cast<CdbRecord*>(newObj);

	return record;
}

} /* namespace codablecash */
