
set(testsrc
	test_instance_spaces.cpp
	test_modular_instance.cpp
	test_smartcontract_executor.cpp
)
handle_sub(testall "${testsrc}" test smartcontract_modular/instance)
