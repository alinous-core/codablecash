

set(__src
	BlockBodyStore.cpp
	BlockBodyStoreCache.cpp
	BlockBodyStoreCacheElement.cpp
	BlockBodyStoreManager.cpp
	BlockMerkleRootIndex.cpp
	FilePositionData.cpp
	FilePositionDataFactory.cpp
)
handle_sub(codablecashlib "${__src}" blockchain bc_blockstore_body)
