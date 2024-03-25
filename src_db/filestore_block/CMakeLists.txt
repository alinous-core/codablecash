
set(__src
	BlockData.cpp
	BlockFileBody.cpp
	BlockFileHeader.cpp
	BlockFileStore.cpp
	BlockHandle.cpp
	exceptions.cpp
	IBlockFileStore.cpp
	IBlockHandle.cpp
)
handle_sub(codablecashlib "${__src}" db filestore_block)
