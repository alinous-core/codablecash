
set(testsrc
	test_json_copy.cpp
	test_json_parser.cpp
	test_json_parser2.cpp
)
handle_sub(testall "${testsrc}" test db/db_json)

