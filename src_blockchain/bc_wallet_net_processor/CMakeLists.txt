

set(__src
	ClientCommandsQueueProcessor.cpp
	NetworkClientCommandListner.cpp
	NetworkClientCommandProcessor.cpp
	NetworkClientCommandProcessorThread.cpp
)
handle_sub(codablecashlib "${__src}" blockchain bc_wallet_net_processor)

