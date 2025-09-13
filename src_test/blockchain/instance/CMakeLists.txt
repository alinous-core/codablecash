
set(testsrc
	test_instance_framework.cpp
	test_scheduled_block.cpp
)
handle_sub(testall "${testsrc}" test blockchain/instance)

