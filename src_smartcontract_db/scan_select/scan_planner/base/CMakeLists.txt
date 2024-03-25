

set(__src
	ConditionsHolder.cpp
	ConditionsHolderStack.cpp
	ConditionsHolderStackMarker.cpp
	GroupByPlanner.cpp
	SelectScanPlanner.cpp
	TablesHolder.cpp
	UpdateColumnPlanner.cpp
	UpdateColumnPolicy.cpp
)
handle_sub(codablecashlib "${__src}" smartcontract_db scan_select/scan_planner/base)

