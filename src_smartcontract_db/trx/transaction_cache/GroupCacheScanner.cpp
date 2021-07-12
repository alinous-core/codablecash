/*
 * GroupCacheScanner.cpp
 *
 *  Created on: 2021/07/12
 *      Author: iizuka
 */

#include "trx/transaction_cache/GroupCacheScanner.h"
#include "trx/transaction_cache/GroupCache.h"

namespace codablecash {

GroupCacheScanner::GroupCacheScanner(GroupCache* gcache) {
	this->gcache = gcache;
}

GroupCacheScanner::~GroupCacheScanner() {
	this->gcache = nullptr;
}

void GroupCacheScanner::init() {

}

} /* namespace codablecash */
