

set(__src
	BlockHeaderValidator.cpp
	BlockValidator.cpp
	BlockValidationException.cpp
)
handle_sub(codablecashlib "${__src}" blockchain bc_block_validator)
