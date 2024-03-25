

set(__src
	AbstractMempoolTransactionScanner.cpp
	AbstractTrxMemoryPool.cpp
	BalanceTransferTrxPool.cpp
	ControlTransactionScanner.cpp
	ControlTrxPool.cpp
	FeeBananceTransactionScanner.cpp
	InterChainCommunicationTransactionScanner.cpp
	InterChainCommunicationTrxPool.cpp
	MemoryPoolException.cpp
	MemoryPool.cpp
	MemPoolTransaction.cpp
	MempoolUtxoFindResult.cpp
	SmartcontractTrxPool.cpp
	SmartcontractTansactionScanner.cpp
)
handle_sub(codablecashlib "${__src}" blockchain bc_memorypool)

