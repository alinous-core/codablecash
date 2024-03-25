
set(testsrc
	test_nop_transactions.cpp
)
handle_sub(testall "${testsrc}" test blockchain/trx)
