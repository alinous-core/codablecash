

set(__src
	AbstractNetworkInfoCommandMessage.cpp
	AbstractNetworkTransferCommandMessage.cpp
	DnsCheckCommandMessage.cpp
	NetworkInfoProcessor.cpp
	NetworkTransferClientNotifyCommand.cpp
	NetworkTransferNodeCommand.cpp
	NetworkTransferProcessor.cpp
	P2pRequestProcessor.cpp
	P2pRequestQueueProcessor.cpp
	P2pRequestQueueProcessorThread.cpp
)
handle_sub(codablecashlib "${__src}" blockchain_p2p bc_p2p_processor)

