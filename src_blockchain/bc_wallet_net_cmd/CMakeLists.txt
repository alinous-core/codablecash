

set(__src
	AbstractClientCommand.cpp
	ClientNewTransactionCommand.cpp
)
handle_sub(codablecashlib "${__src}" blockchain bc_wallet_net_cmd)

