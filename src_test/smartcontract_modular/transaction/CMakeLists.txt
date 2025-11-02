
set(testsrc
	test_smartcontract_address.cpp
)
handle_sub(testall "${testsrc}" test smartcontract_modular/transaction)
