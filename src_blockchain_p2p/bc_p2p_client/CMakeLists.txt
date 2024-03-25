

set(__src
	P2pClient.cpp
	P2pClientConnectionException.cpp
	StackClientListnerRemover.cpp
)
handle_sub(codablecashlib "${__src}" blockchain_p2p bc_p2p_client)

