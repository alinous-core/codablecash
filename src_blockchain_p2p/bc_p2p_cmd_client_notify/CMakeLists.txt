

set(__src
	AbstractClientNotifyCommand.cpp
	ClientExecutor.cpp
	ClientNotifyBlockMinedCommand.cpp
	ClientNotifyNewTransactionCommand.cpp
	IClientNotifyListner.cpp
)
handle_sub(codablecashlib "${__src}" blockchain_p2p bc_p2p_cmd_client_notify)

