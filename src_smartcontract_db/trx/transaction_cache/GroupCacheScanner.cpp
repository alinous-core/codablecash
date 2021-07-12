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


namespace codablecash {

GroupCacheScanner::GroupCacheScanner(GroupCache* gcache) {
	this->gcache = gcache;
	this->scanner = nullptr;
}

GroupCacheScanner::~GroupCacheScanner() {
	this->gcache = nullptr;
	delete this->scanner;
}

void GroupCacheScanner::init() {
	GroupRecordCache* grecordCache = this->gcache->getCache();

	this->scanner = grecordCache->getScanner();
	this->scanner->begin();
}

bool GroupCacheScanner::hasNext() {
	return this->scanner->hasNext();
}

} /* namespace codablecash */
