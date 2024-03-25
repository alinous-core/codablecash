

set(__src
	Block.cpp
	BlockHeader.cpp
	BlockHeaderId.cpp
	BlockMerkleRoot.cpp
	BlockMerkleRootKey.cpp
	BlockMerkleRootKeyFactory.cpp
)
handle_sub(codablecashlib "${__src}" blockchain bc_block)
