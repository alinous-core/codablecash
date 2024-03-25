

set(__src
	AbstractTransactionAcceptionQueueCommand.cpp
	AbstructNodeQueueCommand.cpp
	ClientTransactionAcceptionQueueCommand.cpp
	NodeQueueCommandException.cpp
	NodeTransactionAcceptionQueueCommand.cpp
	NopNodeQueueCommand.cpp
)
handle_sub(codablecashlib "${__src}" blockchain_p2p command_queue_cmd)

