
set(testsrc
	test_insert.cpp
	test_undo_insert.cpp
)
handle_sub(testall "${testsrc}" test smartcontract_db/dml/insert)
