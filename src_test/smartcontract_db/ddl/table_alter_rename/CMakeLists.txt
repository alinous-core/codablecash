
set(testsrc
	test_exec_alter_rename_table_err.cpp
	test_exec_alter_rename_table.cpp
	test_exec_alter_rename_table.cpp
	test_undo_alter_rename_column.cpp
	test_undo_alter_rename_table.cpp
)
handle_sub(testall "${testsrc}" test smartcontract_db/ddl/table_alter_rename)
