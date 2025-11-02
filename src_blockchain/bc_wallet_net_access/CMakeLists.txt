

set(__src
	ClientFetchMempoolTrxAccess.cpp
	IRetriableClientAccess.cpp
	ListStakingNodeAccess.cpp
	RetriableClientAccessContainer.cpp
)
handle_sub(codablecashlib "${__src}" blockchain bc_wallet_net_access)

