
set(testsrc
	test_generic_list.cpp
	test_object_class.cpp
	test_string_class.cpp
)
handle_sub(testall "${testsrc}" test smartcontract_vm/reserved_classes)

