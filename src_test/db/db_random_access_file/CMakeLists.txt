
set(testsrc
	test_random_access_file_length.cpp
	test_random_access_file.cpp
	test_thread_random_access_file.cpp
)
handle_sub(testall "${testsrc}" test db/db_random_access_file)

