
set(testsrc
	test_delete.cpp
	test_undo_delete.cpp
)
handle_sub(testall "${testsrc}" test smartcontract_db/dml/delete)
