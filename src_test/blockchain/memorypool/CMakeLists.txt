
set(testsrc
	test_memory_pool_trx.cpp
	test_memory_pool.cpp
)
handle_sub(testall "${testsrc}" test blockchain/memorypool)

