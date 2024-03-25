

set(__src
	AbstractClientRequestCommand.cpp
	DownloadDnsInfoCommand.cpp
	DownloadDnsInfoCommandResponse.cpp
	PingNodeCommand.cpp
	PingNodeCommandResponse.cpp
	SendTransactionClientCommand.cpp
)
handle_sub(codablecashlib "${__src}" blockchain_p2p bc_p2p_cmd_client)

