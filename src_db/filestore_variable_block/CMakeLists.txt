
set(__src
	VariableBlock.cpp
	VariableBlockFileBody.cpp
	VariableBlockFileStore.cpp
	VariableBlockHandle.cpp
	VariableBlockHeader.cpp
)
handle_sub(codablecashlib "${__src}" db filestore_variable_block)
