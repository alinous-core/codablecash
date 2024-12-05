
set(testsrc
	ClientConnectUtils.cpp
	DebugCodablecashSystemParamSetup.cpp
	InstanceDriver.cpp
	NodeConnectionSimulator.cpp
	TransactionWaiter.cpp
	ValidationInstance.cpp
)
handle_sub(testall "${testsrc}" test blockchain/utils)

