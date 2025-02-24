

set(__src
	AbstractNodeNetworkNotifyCommand.cpp
	NodeNetworkInfo.cpp
	NodeShutdownCommand.cpp
)
handle_sub(codablecashlib "${__src}" blockchain_p2p bc_p2p_cmd_network)

