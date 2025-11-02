
set(testsrc
	test_address.cpp
	test_bloomfilter.cpp
	test_bloomfilter1024_value.cpp
	test_bloomfilter1024.cpp
	test_client_command_queue.cpp
	test_net_wallet.cpp
	test_ripemd160.cpp
	test_unfinalized_repo.cpp
	test_wallet_account_utxo_collector.cpp
	test_wallet_encoder.cpp
	test_wallet_mempool.cpp
	test_wallet_misc.cpp
	test_wallet.cpp
)
handle_sub(testall "${testsrc}" test blockchain/wallet)

