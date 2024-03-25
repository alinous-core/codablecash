
set(testsrc
	TestDbSchema0Index01.cpp
	TestDbSchema1Index01.cpp
	TestDbSchema2Index01.cpp
	TestDbSchema3Index01.cpp
)
handle_sub(testall "${testsrc}" test smartcontract_db/toolkit_index)
