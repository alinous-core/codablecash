
set(testsrc
	test_alter_modify_log_binary.cpp
	test_alter_parse_modify.cpp
	test_database_equals_checker.cpp
	test_exec_alter_modify_err.cpp
	test_exec_alter_modify_index_check.cpp
	test_exec_alter_modify_text_type.cpp
	test_exec_alter_modify_unique.cpp
	test_exec_alter_modify.cpp
	test_undo_alter_modify_index_check.cpp
	test_undo_alter_modify_text_type.cpp
	test_undo_alter_modify_unique.cpp
	test_undo_alter_modify.cpp
	test_undo_misc.cpp
)
handle_sub(testall "${testsrc}" test smartcontract_db/ddl/table_alter_modify)
