
set(testsrc
	ArrayDebugSeeder.cpp
	IDebugSeeder.cpp
	MultizoneTestnet.cpp
	TestnetInstanceWrapper.cpp
	ZoneTestnet.cpp
)
handle_sub(testall "${testsrc}" test blockchain/utils_testnet)

