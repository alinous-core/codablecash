
set(testsrc
	test_placeholder_index.cpp
	test_placeholder.cpp
)
handle_sub(testall "${testsrc}" test smartcontract_db/select/placeholder)
