
set(testsrc
	test_count_func.cpp
	test_groupby.cpp
	test_grouped_records.cpp
)
handle_sub(testall "${testsrc}" test smartcontract_db/select/table_group_by)
