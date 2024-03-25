

set(__src
	BlockValidator.cpp
	BlockValidationException.cpp
)
handle_sub(codablecashlib "${__src}" blockchain bc_block_validator)
