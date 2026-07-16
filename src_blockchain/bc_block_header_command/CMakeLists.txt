

set(__src
	AbstractBlockHeaderCommand.cpp
	NewShardZoneCommand.cpp
)
handle_sub(codablecashlib "${__src}" blockchain bc_block_header_command)
