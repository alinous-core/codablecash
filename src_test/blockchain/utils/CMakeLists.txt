
set(testsrc
	ClientConnectUtils.cpp
	DebugCodablecashConfigSetup.cpp
	InstanceDriver.cpp
	NodeConnectionSimulator.cpp
	TransactionWaiter.cpp
	ValidationInstance.cpp
)
handle_sub(testall "${testsrc}" test blockchain/utils)

