
set(testsrc
	test_node_source.cpp
	test_p2p_client_connection.cpp
	test_p2p_client_ping.cpp
	test_p2p_instance.cpp
	test_p2p_network_cmd.cpp
)
handle_sub(testall "${testsrc}" test p2p/instance)

