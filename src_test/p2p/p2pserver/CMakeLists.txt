
set(testsrc
	test_p2p_client_command.cpp
	test_p2p_pubsub.cpp
	test_p2p_server_connection.cpp
	test_p2p_server.cpp
	test_p2p_v4server_connection.cpp
	test_pubsub_misc.cpp
)
handle_sub(testall "${testsrc}" test p2p/p2pserver)

