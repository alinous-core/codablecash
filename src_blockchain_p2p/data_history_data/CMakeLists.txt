

set(__src
	BlockHeaderTransferData.cpp
	BlockHeaderTransferDataValidator.cpp
	BlockTransactionTransferData.cpp
	DataStructureException.cpp
	TransactionTransferData.cpp
	VoteTransactionIdCertificate.cpp
)
handle_sub(codablecashlib "${__src}" blockchain_p2p data_history_data)

