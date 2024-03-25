
set(testsrc
	test_network_infodb_zone.cpp
	test_network_infodb.cpp
)
handle_sub(testall "${testsrc}" test p2p/netinfo)

