

set(__src
	AbstractClientQueueCommand.cpp
	ClientBlockMinedCommand.cpp
	ClientNewTransactionCommand.cpp
	FetchHeaderTransactionsWorker.cpp
)
handle_sub(codablecashlib "${__src}" blockchain bc_wallet_net_cmd)

