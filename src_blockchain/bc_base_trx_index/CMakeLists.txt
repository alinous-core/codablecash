

set(__src
	BalanceUnitKey.cpp
	BalanceUnitKeyFactory.cpp
	TransactionData.cpp
	TransactionDataFactory.cpp
	TransactionIdData.cpp
	TransactionIdDataFactory.cpp
	TransactionIdKey.cpp
	TransactionIdKeyFactory.cpp
	TransactionIdsListData.cpp
	TransactionIdsListDataFactory.cpp
)
handle_sub(codablecashlib "${__src}" blockchain bc_base_trx_index)

