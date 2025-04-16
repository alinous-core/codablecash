

set(__src
	BloomFilter.cpp
	BloomFilter512.cpp
)
handle_sub(codablecashlib "${__src}" blockchain bc_wallet_filter)

