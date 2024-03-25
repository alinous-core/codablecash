
set(testsrc
	test_index_selection.cpp
	test_multi_range_index_select.cpp
)
handle_sub(testall "${testsrc}" test smartcontract_db/select/index_select)
