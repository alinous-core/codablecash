
set(testsrc
	test_index_or_scanner.cpp
	test_select01.cpp
)
handle_sub(testall "${testsrc}" test smartcontract_db/select/select_stmt)
