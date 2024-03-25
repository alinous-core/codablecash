
set(testsrc
	test_consumed_utxo.cpp
	test_control_trx.cpp
	test_finalizer_misc.cpp
	test_register_vote_ticket.cpp
	test_register_votepool.cpp
	test_vote_and_finalize.cpp
	test_vote_revoke_validation.cpp
	test_vote_revoke.cpp
)
handle_sub(testall "${testsrc}" test blockchain/finalizer)

