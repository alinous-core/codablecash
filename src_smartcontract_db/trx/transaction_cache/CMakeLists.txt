

set(__src
	AbstractSwapCache.cpp
	CdbSwapCacheFactory.cpp
	GroupCache.cpp
	GroupRecordCache.cpp
	OidKeyRecordCache.cpp
	SingleKeyOidCache.cpp
	SwapCacheFactory.cpp
)
handle_sub(codablecashlib "${__src}" smartcontract_db trx/transaction_cache)

