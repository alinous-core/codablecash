

set(__src
	BlockchainStoreException.cpp
	CodablecashBlockchain.cpp
	IBlockchainEventListner.cpp
	ZoneStore.cpp
)
handle_sub(codablecashlib "${__src}" blockchain bc_blockstore)
