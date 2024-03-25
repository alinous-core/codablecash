

set(__src
	CountSQLFunction.cpp
	CountSQLFunctionCache.cpp
)
handle_sub(codablecashlib "${__src}" smartcontract_db scan_select/scan_condition/func_aggregate)

