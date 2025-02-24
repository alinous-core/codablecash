
set(testsrc
	test_address.cpp
	test_net_wallet.cpp
	test_ripemd160.cpp
	test_wallet_encoder.cpp
	test_wallet_misc.cpp
	test_wallet.cpp
)
handle_sub(testall "${testsrc}" test blockchain/wallet)

