

set(__src
	AbstractSmartcontractTransaction.cpp
	NopSmartcontractTransaction.cpp
)
handle_sub(codablecashlib "${__src}" blockchain bc_smartcontract)

