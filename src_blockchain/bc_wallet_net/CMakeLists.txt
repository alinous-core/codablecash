

set(__src
	NetworkTransactionHandler.cpp
	NetworkWallet.cpp
	StakingEncriptedSeed.cpp
	StakingSeedManager.cpp
	WalletConfig.cpp
	WalletConnectionManager.cpp
	WalletNetworkManager.cpp
	WalletNetworkNodeCandidates.cpp
)
handle_sub(codablecashlib "${__src}" blockchain bc_wallet_net)

