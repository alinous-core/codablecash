

set(__src
	CommandQueue.cpp
	CommandQueueData.cpp
	CommandQueueDataFactory.cpp
)
handle_sub(codablecashlib "${__src}" blockchain_p2p command_queue)

