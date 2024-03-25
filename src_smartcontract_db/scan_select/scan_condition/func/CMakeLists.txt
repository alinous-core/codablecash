

set(__src
	AbstractSQLAggregateFunctionBody.cpp
	AbstractSQLFunctionBody.cpp
	SQLFunctionRegistory.cpp
)
handle_sub(codablecashlib "${__src}" smartcontract_db scan_select/scan_condition/func)

