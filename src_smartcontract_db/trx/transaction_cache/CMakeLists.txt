

set(__src
	AbstractSwapCache.cpp
	CdbSwapCacheFactory.cpp
	GroupCache.cpp
	GroupCacheScanner.cpp
	GroupRecordCache.cpp
	OidKeyRecordCacheScanner.cpp
	OidKeyRecordCache.cpp
	RecordScanIdentityCache.cpp
	SingleKeyOidCache.cpp
	SwapCacheFactory.cpp
)
handle_sub(codablecashlib "${__src}" smartcontract_db trx/transaction_cache)

