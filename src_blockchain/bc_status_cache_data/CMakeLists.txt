

set(__src
	FinalizedDataCache.cpp
	FinalizedUtxoRepository.cpp
	FinalizedVoterRepository.cpp
)
handle_sub(codablecashlib "${__src}" blockchain bc_status_cache_data)

