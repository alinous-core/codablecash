
set(testsrc
	test_groupby.cpp
)
handle_sub(testall "${testsrc}" test smartcontract_db/select/table_group_by)
