

set(__src
	AbstractFinalizerCommandMessage.cpp
	DetectVotingTicketCommandMessage.cpp
	FinalizerPool.cpp
)
handle_sub(codablecashlib "${__src}" blockchain bc_finalizer_pool)

