
set(testsrc
	test_create_ids.cpp
	test_node_command_queue.cpp
	test_transfer_history.cpp
)
handle_sub(testall "${testsrc}" test p2p/parts)

