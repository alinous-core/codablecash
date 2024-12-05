

set(__src
	FinalizingCommandMessage.cpp
	FinalizingHeaderCommandMessage.cpp
	MinerMinedReportCommandMessage.cpp
	TransferedMinedReportCommandMessage.cpp
)
handle_sub(codablecashlib "${__src}" blockchain bc_processor_cmd)

