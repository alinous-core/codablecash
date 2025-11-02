

set(__src
	ManagedUtxoCache.cpp
	ManagedUtxoCacheRecord.cpp
	ManagementAccount.cpp
	ManagementAccountsCollection.cpp
	ManagementTransactionsHistory.cpp
)
handle_sub(codablecashlib "${__src}" blockchain bc_wallet_net_data_management)

