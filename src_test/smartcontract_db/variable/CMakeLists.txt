
set(testsrc
	test_dom_array_base.cpp
	test_dom_array_member.cpp
	test_dom_array.cpp
	test_dom_base.cpp
	test_dom_extobj.cpp
	test_dom_member_variable.cpp
	test_dom_parser.cpp
	test_dom_variable.cpp
	test_strict_equals.cpp
	test_vm_bitset.cpp
	test_vm_hash.cpp
	test_vm_mem_list_sort.cpp
)
handle_sub(testall "${testsrc}" test smartcontract_db/variable)
