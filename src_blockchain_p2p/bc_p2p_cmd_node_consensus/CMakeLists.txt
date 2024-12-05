

set(__src
	AbstractConsensusNodeCommand.cpp
	NodeHostory.cpp
	ReportMinedBlockNodeCommand.cpp
	ReportNonceCalculatedNodeCommand.cpp
	SendTransactionNodeCommand.cpp
	SendVoteTransactionNodeCommand.cpp
)
handle_sub(codablecashlib "${__src}" blockchain_p2p bc_p2p_cmd_node_consensus)

