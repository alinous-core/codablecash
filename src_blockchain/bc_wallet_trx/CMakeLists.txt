

set(__src
	AbstractWalletTransactionHandler.cpp
	BalanceTransactionWalletHandler.cpp
	GenesisTransactionHandler.cpp
	RegisterTicketTransactionWalletHandler.cpp
	RegisterVotePoolTransactionWalletHandler.cpp
)
handle_sub(codablecashlib "${__src}" blockchain bc_wallet_trx)

