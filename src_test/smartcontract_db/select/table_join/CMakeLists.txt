
set(testsrc
	test_joinpart_3tables.cpp
	test_joinpart_cross.cpp
	test_joinpart_inner.cpp
	test_joinpart_left.cpp
	test_joinpart_misc.cpp
	test_joinpart_right.cpp
	test_or_join_3tables.cpp
	test_or_join_inner.cpp
	test_or_join_right.cpp
	test_or_join.cpp
)
handle_sub(testall "${testsrc}" test smartcontract_db/select/table_join)
