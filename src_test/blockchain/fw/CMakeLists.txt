
set(testsrc
	DummyErrorNopCommand.cpp
	test_actor.cpp
	test_config_saver.cpp
	test_multiple_actor.cpp
	test_software_version.cpp
)
handle_sub(testall "${testsrc}" test blockchain/fw)

