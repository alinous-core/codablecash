

set(__src
	AbstractFinalizerTransaction.cpp
	AbstractRevokeTransaction.cpp
	RegisterTicketTransaction.cpp
	RegisterVotePoolTransaction.cpp
	RevokeMissedTicket.cpp
	RevokeMissVotedTicket.cpp
	TicketUtxo.cpp
	TicketVotedUtxo.cpp
	TicketVotedUtxoReference.cpp
	TicketUtxoReference.cpp
	VoteBlockTransaction.cpp
)
handle_sub(codablecashlib "${__src}" blockchain bc_finalizer_trx)

