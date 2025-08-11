

set(__src
	AbstractClientQueueCommand.cpp
	ClientNewTransactionCommand.cpp
)
handle_sub(codablecashlib "${__src}" blockchain bc_wallet_net_cmd)

