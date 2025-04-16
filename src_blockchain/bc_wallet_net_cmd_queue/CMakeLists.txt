

set(__src
	ClientCommandsQueue.cpp
	ClientCommandsQueueData.cpp
	ClientCommandsQueueDataFactory.cpp
	ClientCommandsQueueFactory.cpp
	PendingClientCommandsQueue.cpp
)
handle_sub(codablecashlib "${__src}" blockchain bc_wallet_net_cmd_queue)

