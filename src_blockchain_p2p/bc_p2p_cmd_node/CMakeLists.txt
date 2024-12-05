

set(__src
	AbstractNodeCommand.cpp
	DownloadBlockNodeCommand.cpp
	DownloadBlockNodeCommandResponse.cpp
	DownloadOmittedBlockBodyNodeCommand.cpp
	DownloadOmittedBlockBodyNodeCommandResponse.cpp
	DownloadTransactionEntry.cpp
	DownloadTransactionsNodeCommand.cpp
	DownloadTransactionsNodeCommandResponse.cpp
	InvalidTransactionException.cpp
	NodeCommandValidationErrorResponse.cpp
	NopNodeCommand.cpp
	SyncHeaderHeightData.cpp
	SyncHeaderNodeCommand.cpp
	SyncHeaderNodeCommandResponse.cpp
	SyncMempoolNodeCommand.cpp
	SyncMempoolNodeCommandResponse.cpp
)
handle_sub(codablecashlib "${__src}" blockchain_p2p bc_p2p_cmd_node)

