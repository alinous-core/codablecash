
set(testsrc
	test_ipv6_socket.cpp
)
handle_sub(testall "${testsrc}" test p2p/socket)

