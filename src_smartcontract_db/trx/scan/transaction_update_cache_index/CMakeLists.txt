

set(__src
	TransactionTableIndexWrapper.cpp
	TransactionTableUpdateCacheIndexes.cpp
	UpdateCacheIndexScanner.cpp
)
handle_sub(codablecashlib "${__src}" smartcontract_db trx/scan/transaction_update_cache_index)

