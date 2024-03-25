/*
 * UpdateCacheIndexScanner.cpp
 *
 *  Created on: 2022/01/14
 *      Author: iizuka
 */

#include "trx/scan/transaction_update_cache_index/UpdateCacheIndexScanner.h"

#include "trx/transaction_cache/SingleKeyOidCache.h"
#include "trx/transaction_cache/OidKeyRecordCache.h"

#include "schema_table/record/table_record_key/CdbOidKey.h"
#include "schema_table/record/table_record_key/CdbRecordKey.h"

#include "btree/IBtreeScanner.h"

#include "schema_table/record/table_record_value/CdbOidValueListCursor.h"
#include "schema_table/record/table_record_value/CdbOidValueList.h"

#include "base/StackRelease.h"

namespace codablecash {

UpdateCacheIndexScanner::UpdateCacheIndexScanner(SingleKeyOidCache* keyOidCache,
		const CdbRecordKey* begin, bool beginEq, const CdbRecordKey* end, bool endEq, OidKeyRecordCache* recordCache)
			: RangeChecker(begin, beginEq, end, endEq) {
	this->keyOidCache = keyOidCache;
	this->recordCache = recordCache;

	this->scanner = nullptr;
	this->cursor = nullptr;
	this->nextObj = nullptr;
}

UpdateCacheIndexScanner::~UpdateCacheIndexScanner() {
	this->keyOidCache = nullptr;
	this->recordCache = nullptr;
	this->nextObj = nullptr;

	if(this->scanner != nullptr){
		delete this->scanner;
		this->scanner = nullptr;
		delete this->cursor;
		this->cursor = nullptr;
	}
}

void UpdateCacheIndexScanner::init() {
	AbstractBtreeKey* first = getFirstScanKey();

	this->scanner = this->keyOidCache->getScanner();
	this->scanner->begin(first);
}

bool UpdateCacheIndexScanner::hasNext() {
	while(this->cursor == nullptr || !this->cursor->hasNext()){
		if(this->scanner->hasNext()){
			delete this->cursor;
			this->cursor = nullptr;

			const AbstractBtreeKey* key = this->scanner->nextKey();
			const CdbRecordKey* recordKey = dynamic_cast<const CdbRecordKey*>(key);
			if(!checkLowerBound(recordKey)){
				continue;
			}
			if(!checkUpperBound(recordKey)){
				return false;
			}

			const IBlockObject* obj = this->scanner->next();
			const CdbOidValueList* list = dynamic_cast<const CdbOidValueList*>(obj);

			this->cursor = new CdbOidValueListCursor(list);
			this->cursor->hasNext(); // load next
		}
		else{
			return false;
		}
	}

	this->nextObj = this->cursor->next();

	return true;

}

const CdbRecord* UpdateCacheIndexScanner::next() {
	CdbOidKey* key = dynamic_cast<CdbOidKey*>(this->nextObj->toKey()); __STP(key);

	const CdbRecord* record = this->recordCache->findRecord(key);
	return record;
}

} /* namespace codablecash */
