
set(testsrc
	test_nop_transactions.cpp
	test_ticket_transaction_wallet.cpp
)
handle_sub(testall "${testsrc}" test blockchain/trx)
