
set(testsrc
	test_json_parser.cpp
)
handle_sub(testall "${testsrc}" test db/db_json)

