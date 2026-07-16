

set(__src
	AbstarctVoteLockinOperation.cpp
	AbstractLockinOperation.cpp
	ILockinManager.cpp
	LockinManager.cpp
	LockInOperationsData.cpp
	LockInOperationsDataFactory.cpp
	MissvoteLockinOperation.cpp
	VotedLockinOperation.cpp
)
handle_sub(codablecashlib "${__src}" blockchain bc_status_cache_lockin)

