
set(testsrc
	TransactionUtxoChecker.cpp
	WalletDriver.cpp
)
handle_sub(testall "${testsrc}" test blockchain/wallet_util)

