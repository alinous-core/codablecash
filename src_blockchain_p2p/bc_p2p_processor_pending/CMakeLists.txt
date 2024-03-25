

set(__src
	PendingCommandData.cpp
	PendingCommandDataFactory.cpp
	PendingProcessorCommandsQueue.cpp
)
handle_sub(codablecashlib "${__src}" blockchain_p2p bc_p2p_processor_pending)

