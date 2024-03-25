

set(__src
	BlockGenerator.cpp
	BlockRewardCalculator.cpp
	BlockRewardMiningBase.cpp
	BlockRewardStakeBase.cpp
	MiningConfig.cpp
)
handle_sub(codablecashlib "${__src}" blockchain bc_block_generator)
