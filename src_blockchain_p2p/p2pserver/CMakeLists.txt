

set(__src
	AbstractConnectionCommand.cpp
	ConnectPublisherCommand.cpp
	ConnectSubscriberCommand.cpp
	NopConnectionCommand.cpp
	P2pConnectionAcceptThread.cpp
	P2pConnectionListeningThread.cpp
	P2pServer.cpp
	P2pServerConnectionManager.cpp
	WrongPubsubConnectionCommandException.cpp
)
handle_sub(codablecashlib "${__src}" blockchain_p2p p2pserver)

