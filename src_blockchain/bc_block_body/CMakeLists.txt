

set(__src
	AbstractBlockRewordTransaction.cpp
	BlockBody.cpp
	BlockRewordBase.cpp
	Coinbase.cpp
	CoinbaseTransaction.cpp
	ITransactionVisitor.cpp
	OmittedBlockBody.cpp
	Stakebase.cpp
	StakeBaseTransaction.cpp
)
handle_sub(codablecashlib "${__src}" blockchain bc_block_body)
