
set(testsrc
	test_modular_instance.cpp
	test_smartcontract_executor.cpp
)
handle_sub(testall "${testsrc}" test smartcontract_modular/instance)
