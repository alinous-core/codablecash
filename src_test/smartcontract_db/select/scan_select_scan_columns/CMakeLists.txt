
set(testsrc
	test_scan_columns_arithmetic.cpp
	test_scan_columns_exp.cpp
	test_scan_columns_funcs.cpp
	test_scan_columns_logical.cpp
	test_scan_const_params.cpp
	test_scan_misc.cpp
)
handle_sub(testall "${testsrc}" test smartcontract_db/select/scan_select_scan_columns)
