

set(__src
	AbstractCentralProcessorCommandMessage.cpp
	CentralProcessor.cpp
)
handle_sub(codablecashlib "${__src}" blockchain bc_processor)

