
set(testsrc
	test_block_filestore.cpp
	test_filestore.cpp
	test_longrange.cpp
	test_longrange_binary.cpp
	test_variable_block_filestore_err.cpp
	test_variable_block_filestore.cpp
	test_variable_block_filestore2.cpp
)
handle_sub(testall "${testsrc}" test db/db_filestore)

