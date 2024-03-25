

set(__src
	IWalletDataEncoder.cpp
	PasswordEncoder.cpp;
)
handle_sub(codablecashlib "${__src}" blockchain bc_wallet_encoder)

