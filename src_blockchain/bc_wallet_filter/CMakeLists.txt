

set(__src
	BloomFilter.cpp
	BloomFilter1024.cpp
	BloomHash1024.cpp
)
handle_sub(codablecashlib "${__src}" blockchain bc_wallet_filter)

