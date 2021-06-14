

set(__src
	OidArrayCache.cpp
)
handle_sub(codablecashlib "${__src}" smartcontract_db trx/transaction_cache_array)

