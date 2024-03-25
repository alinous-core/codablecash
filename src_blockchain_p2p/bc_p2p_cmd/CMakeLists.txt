

set(__src
	AbstractBlockchainLoginCommand.cpp
	AbstractBlockchainPubsubCommand.cpp
	LoginClientCommand.cpp
	LoginErrorResponse.cpp
	LoginPubSubCommand.cpp
	P2pHandshakeAuthenticationException.cpp
	SignatureErrorResponse.cpp
	SignatureVerifivcationErrorException.cpp
)
handle_sub(codablecashlib "${__src}" blockchain_p2p bc_p2p_cmd)

