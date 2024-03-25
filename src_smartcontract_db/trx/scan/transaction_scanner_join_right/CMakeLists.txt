

set(__src
	AbstractRightTransactionScanner.cpp
	IRightOrTransactionScanner.cpp
	RightTableBufferedOidTransactionScanner.cpp
	RightTableIndexTransactionScanner.cpp
	RightTableBufferedTransactionScanner.cpp
	RightTableOrTransactionScanner.cpp
	RightWrappedLeftOrTransactionScanner.cpp
	RightWrappedLeftTransactionScanner.cpp
)
handle_sub(codablecashlib "${__src}" smartcontract_db trx/scan/transaction_scanner_join_right)

