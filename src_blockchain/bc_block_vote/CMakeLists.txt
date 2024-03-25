

set(__src
	VotedHeaderIdGroup.cpp
	VotePart.cpp
)
handle_sub(codablecashlib "${__src}" blockchain bc_block_vote)
