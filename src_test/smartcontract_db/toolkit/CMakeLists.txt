
set(testsrc
	TestDbSchema01.cpp
	TestDbSchema02.cpp
	TestDbSchema03.cpp
	TestDbSchema4Cost01.cpp
	TestDbSchemaBase.cpp
)
handle_sub(testall "${testsrc}" test smartcontract_db/toolkit)
