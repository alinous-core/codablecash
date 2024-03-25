
set(testsrc
	test_exec_alter_index_add_err.cpp
	test_exec_alter_index_drop_err.cpp
	test_exec_alter_index.cpp
	test_undo_alter_index_add.cpp
	test_undo_alter_index_drop.cpp
)
handle_sub(testall "${testsrc}" test smartcontract_db/ddl/table_alter_index)
