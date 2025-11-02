

set(__src
	HeaderTransactionGroup.cpp
	HeaderTransactionGroupDataFactory.cpp
	NetworkWalletData.cpp
	TransactionGroupDataStore.cpp
	WalletMemoryPool.cpp
)
handle_sub(codablecashlib "${__src}" blockchain bc_wallet_net_data)

