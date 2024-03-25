

set(__src
	FinalizingCommandMessage.cpp
	FinalizingHeaderCommandMessage.cpp
	MinedReportCommandMessage.cpp
	TransferedMinedReportCommandMessage.cpp
)
handle_sub(codablecashlib "${__src}" blockchain bc_processor_cmd)

