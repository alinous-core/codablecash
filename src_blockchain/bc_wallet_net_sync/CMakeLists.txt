

set(__src
	NetworkWalletSync.cpp
	WalletHeaderSyncWorker.cpp
)
handle_sub(codablecashlib "${__src}" blockchain bc_wallet_net_sync)

