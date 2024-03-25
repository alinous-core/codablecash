

set(__src
	BlockHeaderHeightIndex.cpp
	BlockHeaderHeightIndexDataFactory.cpp
	BlockHeaderHeightIndexKey.cpp
	BlockHeaderHeightIndexKeyFactory.cpp
	BlockHeaderIdKey.cpp
	BlockHeaderIdKeyFactory.cpp
	BlockHeaderList.cpp
	BlockHeaderNotFoundException.cpp
	BlockHeaderStore.cpp
	BlockHeaderStoreCache.cpp
	BlockHeaderStoreCacheElement.cpp
	BlockHeaderStoreManager.cpp
	IHeaderRemovalNotifier.cpp
)
handle_sub(codablecashlib "${__src}" blockchain bc_blockstore_header)
