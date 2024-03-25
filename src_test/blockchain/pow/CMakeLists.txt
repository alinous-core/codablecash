
set(testsrc
	test_pow_manager.cpp
	test_pow_nonce.cpp
	test_pow_random_hash.cpp
	TestBlockGenerator.cpp
)
handle_sub(testall "${testsrc}" test blockchain/pow)

