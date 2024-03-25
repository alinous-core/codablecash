

set(__src
	AbstractColumnsIndexAccess.cpp
	IndexResolver.cpp
	IndexScorer.cpp
	MultipleColumnIndexAccess.cpp
	OrIndexAccessCollection.cpp
	SingleColumnIndexAccess.cpp
)
handle_sub(codablecashlib "${__src}" smartcontract_db scan_select/scan_planner/scanner/index_resolv)

