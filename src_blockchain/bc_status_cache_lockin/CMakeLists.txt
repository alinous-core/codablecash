

set(__src
	AbstractLockinOperation.cpp
	LockinManager.cpp
	MissvoteLockinOperation.cpp
	VotedLockinOperation.cpp
)
handle_sub(codablecashlib "${__src}" blockchain bc_status_cache_lockin)

