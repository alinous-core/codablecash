

set(__src
	AbstractCommandResponse.cpp
	AbstractPubSubCommand.cpp
	EchoPubSubCommand.cpp
	EchoPubSubCommandResponse.cpp
	EndHandshakeCommand.cpp
	ErrorPubsubResponse.cpp
	OkPubsubResponse.cpp
	WarningResponse.cpp
)
handle_sub(codablecashlib "${__src}" blockchain_p2p pubsub_cmd)

