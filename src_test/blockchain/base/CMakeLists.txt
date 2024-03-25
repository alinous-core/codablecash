
set(testsrc
	test_32bytes_id.cpp
	test_balance_unit.cpp
	test_base58.cpp
	test_dummy_utxo.cpp
	test_exceptions.cpp
	test_merkletree.cpp
)
handle_sub(testall "${testsrc}" test blockchain/base)

