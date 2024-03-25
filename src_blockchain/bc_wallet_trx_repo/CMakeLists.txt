

set(__src
	WalletAccountTrxRepository.cpp
	WalletAccountUtxoRepository.cpp
)
handle_sub(codablecashlib "${__src}" blockchain bc_wallet_trx_repo)

