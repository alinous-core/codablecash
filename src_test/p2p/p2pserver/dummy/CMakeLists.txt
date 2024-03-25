
set(testsrc
	DebugP2pHandShake.cpp
	DummyClientListner.cpp
	DummyServer.cpp
	DummyThread01.cpp
	DummyThread02.cpp
	DummyThread03.cpp
)
handle_sub(testall "${testsrc}" test p2p/p2pserver/dummy/)

