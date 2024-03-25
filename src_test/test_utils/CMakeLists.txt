
set(testsrc
	Check.cpp
	TestBreak.cpp
	TestCase.cpp
	TestEnv.cpp
	TestExecutor.cpp
	TestGroup.cpp
	TestGroupActions.cpp
	TestParams.cpp
	TestPortSelection.cpp
	TestSummary.cpp
)
handle_sub(testall "${testsrc}" test test_utils)


