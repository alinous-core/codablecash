

set(__src
	IRetryableResponceValidator.cpp
	RetryableNode2NodeAccess.cpp
	RetryableNode2NodeAccessException.cpp
)
handle_sub(codablecashlib "${__src}" blockchain_p2p bc_p2p_cmd_node_access)

