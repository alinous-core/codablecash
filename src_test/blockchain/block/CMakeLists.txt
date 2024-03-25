
set(testsrc
	test_block_body_store_manager.cpp
	test_block_body.cpp
	test_block_genesis.cpp
	test_header_store_misc.cpp
	test_header_store.cpp
)
handle_sub(testall "${testsrc}" test blockchain/block)

