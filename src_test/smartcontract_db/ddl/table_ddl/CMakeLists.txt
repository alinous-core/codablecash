
set(testsrc
	test_create_table_error.cpp
	test_create_table_reload.cpp
	test_create_table_script.cpp
	test_create_table.cpp
	test_drop_table.cpp
	test_undo_create_table.cpp
	test_undo_drop_table.cpp
)
handle_sub(testall "${testsrc}" test smartcontract_db/ddl/table_ddl)
