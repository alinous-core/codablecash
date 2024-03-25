
set(testsrc
	test_undo_update.cpp
	test_update_inserted.cpp
	test_update.cpp
)
handle_sub(testall "${testsrc}" test smartcontract_db/dml/update)
