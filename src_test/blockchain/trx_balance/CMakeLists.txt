
set(testsrc
	test_address_data.cpp
	test_balance_trx.cpp
	test_genesis_trx.cpp
)
handle_sub(testall "${testsrc}" test blockchain/trx_balance)

