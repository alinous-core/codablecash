

set(__src
	AbstractNodeNetworkNotifyCommand.cpp
	NodeLiveCommand.cpp
	NodeLiveData.cpp
	NodeShutdownCommand.cpp
)
handle_sub(codablecashlib "${__src}" blockchain_p2p bc_p2p_cmd_network)

