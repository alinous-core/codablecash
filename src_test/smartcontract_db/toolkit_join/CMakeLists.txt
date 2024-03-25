
set(testsrc
	TestDbSchema3TablesJoin.cpp
	TestDbSchemaJoin01.cpp
	TestDbSchemaOrJoin01.cpp
)
handle_sub(testall "${testsrc}" test smartcontract_db/toolkit_join)
