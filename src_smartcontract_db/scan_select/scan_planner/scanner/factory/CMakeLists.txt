

set(__src
	AbstractJoinScannerFactory.cpp
	AbstractScannerFactory.cpp
	CrossJoinScanerFactory.cpp
	InnerJoinScannerFactory.cpp
	LeftJoinScannerFactory.cpp
	RightJoinScannerFactory.cpp
	TableScannerFactory.cpp
)
handle_sub(codablecashlib "${__src}" smartcontract_db scan_select/scan_planner/scanner/factory)

