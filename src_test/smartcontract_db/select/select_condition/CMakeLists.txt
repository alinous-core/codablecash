
set(testsrc
	test_cast_err.cpp
	test_condition_misc.cpp
	test_like_condition.cpp
	test_exp_condition.cpp
	test_func_condition.cpp
	test_like_processor.cpp
	test_select_arithmetic_contition.cpp
	test_select_arithmetic_contition2.cpp
	test_select_contition.cpp
	test_select_contition02.cpp
	test_select_logic_contition.cpp
)
handle_sub(testall "${testsrc}" test smartcontract_db/select/select_condition)
