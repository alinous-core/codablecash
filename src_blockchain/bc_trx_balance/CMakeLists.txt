

set(__src
	BalanceShortageException.cpp
	BalanceTransferTransaction.cpp
	BalanceUtxo.cpp
	BalanceUtxoReference.cpp
	BalanceUtxoSign.cpp
	FeeShortageException.cpp
	IMuSigSignerProvidor.cpp
	InputUtxoCollection.cpp
	IUtxoFinder.cpp
)
handle_sub(codablecashlib "${__src}" blockchain bc_trx_balance)
