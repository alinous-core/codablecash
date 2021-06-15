

set(__src
	OidArrayCache.cpp
	OidArrayCacheElement.cpp
)
handle_sub(codablecashlib "${__src}" smartcontract_db trx/transaction_cache_array)

