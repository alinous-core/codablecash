

set(__src
	AbstractJoinExecutor.cpp
	CrossJoinExecutor.cpp
	InnerJoinExecutor.cpp
	IJoinLeftSource.cpp
	IJoinRightSource.cpp
	JoinCandidateCursor.cpp
	OuterJoinExecutor.cpp
	RightJoinExecutor.cpp
)
handle_sub(codablecashlib "${__src}" smartcontract_db trx/scan/transaction_scanner_join)

