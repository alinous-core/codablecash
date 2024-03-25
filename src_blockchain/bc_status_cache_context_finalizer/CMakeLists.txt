

set(__src
	AlreadyFinalizedException.cpp
	VoteCandidate.cpp
	VoterStatusCacheContext.cpp
	VotingBlockStatus.cpp
	VotingBlockStatusDataFactory.cpp
	VotingBlockStatusRepository.cpp
)
handle_sub(codablecashlib "${__src}" blockchain bc_status_cache_context_finalizer)

