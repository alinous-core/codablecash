

set(__src
	NetworkWallet.cpp
	NetworkWalletData.cpp
	StakingSeedManager.cpp
	WalletConnectionManager.cpp
	WalletNetworkManager.cpp
	WalletNetworkNodeCandidates.cpp
)
handle_sub(codablecashlib "${__src}" blockchain bc_wallet_net)

