

set(__src
	ArrayUtxoFinder.cpp
	HdWalletAccountTrxBuilderContext.cpp
	HdWalletAccountUtxoCollector.cpp
	ITransactionBuilderContext.cpp
	IUtxoCollector.cpp
	NetWalletAccountTrxBuilderContext.cpp
	NetWalletAccountUtxoArray.cpp
	NetWalletAccountUtxoCollector.cpp
)
handle_sub(codablecashlib "${__src}" blockchain bc_wallet_trx_base)

