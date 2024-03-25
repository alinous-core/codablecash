

set(__src
	AbstractAddressStore.cpp
	AbstractWalletAccount.cpp
	AddressAndPrivateKey.cpp
	ChangeAddressStore.cpp
	HdWalleMuSigSignerProvidor.cpp
	HdWalletSeed.cpp
	HdWallet.cpp
	ReceivingAddressStore.cpp
	WalletAccount.cpp
)
handle_sub(codablecashlib "${__src}" blockchain bc_wallet)

