/*
 * CdbSwapCacheFactory.h
 *
 *  Created on: 2020/09/24
 *      Author: iizuka
 */

#ifndef TRANSACTION_CACHE_CDBSWAPCACHEFACTORY_H_
#define TRANSACTION_CACHE_CDBSWAPCACHEFACTORY_H_

#include "trx/transaction_cache/SwapCacheFactory.h"

namespace codablecash {

class CdbKeyFactory;
class CdbDataFactory;
class SingleKeyOidCache;
class OidKeyRecordCache;
class GroupRecordCache;
class OidArrayCache;
class CountSQLFunctionCache;
class RecordScanIdentityCache;

class CdbSwapCacheFactory: public SwapCacheFactory {
public:
	static const UnicodeString PREFIX_SINGLE_KEY;
	static const UnicodeString PREFIX_OID_KEY;
	static const UnicodeString PREFIX_GROUP_KEY;
	static const UnicodeString PREFIX_OID_ARRAY_KEY;
	static const UnicodeString PREFIX_COUNT_SQL_FUNCTION_KEY;
	static const UnicodeString PREFIX_RECORD_SCAN_IDENTITY_KEY;

	CdbSwapCacheFactory(const File* tmpdir, DiskCacheManager* diskCache);
	virtual ~CdbSwapCacheFactory();

	virtual UnicodeString* getName() noexcept;

	SingleKeyOidCache* createSingleKeyOidCache(int swappiness);
	OidKeyRecordCache* createOidKeyRecordCache(int swappiness);
	GroupRecordCache* createGroupRecordCache(int swappiness);
	OidArrayCache* createOidArrayCache(int swappiness);
	CountSQLFunctionCache* createCountSQLFunctionCache(int swappiness);
	RecordScanIdentityCache* createRecordScanIdentityCache(int swappiness);
};

} /* namespace codablecash */

#endif /* TRANSACTION_CACHE_CDBSWAPCACHEFACTORY_H_ */
