

set(__src
	NetworkWallet.cpp
	WalletConnectionManager.cpp
	WalletNetworkManager.cpp
)
handle_sub(codablecashlib "${__src}" blockchain bc_wallet_net)

