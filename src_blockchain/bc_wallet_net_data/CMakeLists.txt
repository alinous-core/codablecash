

set(__src
	HeaderTransactionGroup.cpp
	HeaderTransactionGroupDataFactory.cpp
	NetworkWalletData.cpp
	TransactionGroupDataStore.cpp
	WalletMemoryPool.cpp
	WalletMemoryPoolScanner.cpp
	WalletTransactionIdListData.cpp
	WalletTransactionIdListDataFactory.cpp
)
handle_sub(codablecashlib "${__src}" blockchain bc_wallet_net_data)

