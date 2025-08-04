

set(__src
	AbstractBalanceTransaction.cpp
	AbstractBlockchainTransaction.cpp
	AbstractControlTransaction.cpp
	AbstractInterChainCommunicationTansaction.cpp
	AbstractUtxo.cpp
	AbstractUtxoReference.cpp
	IAddressChecker.cpp
	IUtxoRefChecker.cpp
	NopInterChainCommunicationTransaction.cpp
	TransactionId.cpp
	UtxoId.cpp
)
handle_sub(codablecashlib "${__src}" blockchain bc_trx)
