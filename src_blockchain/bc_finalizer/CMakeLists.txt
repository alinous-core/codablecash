

set(__src
	TicketVoteSelector.cpp
	VoterEntry.cpp
	VoterEntryFactory.cpp
	VoteTicket.cpp
)
handle_sub(codablecashlib "${__src}" blockchain bc_finalizer)

