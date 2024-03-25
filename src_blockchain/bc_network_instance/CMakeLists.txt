

set(__src
	ArrayNetworkSheeder.cpp
	CodablecashNetworkNode.cpp
	CodablecashNetworkNodeConfig.cpp
	FinalizerConfig.cpp
	GenesisBalanceConfig.cpp
	INetworkSeeder.cpp
)
handle_sub(codablecashlib "${__src}" blockchain bc_network_instance)

