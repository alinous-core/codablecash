
set(testsrc
	test_first_net.cpp
	test_omitted_body.cpp
	test_pending_request_processor.cpp
	test_sync_block.cpp
	test_sync_header_only.cpp
	test_sync_mempool.cpp
)
handle_sub(testall "${testsrc}" test blockchain/testnet)
