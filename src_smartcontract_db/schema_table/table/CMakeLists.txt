

set(__src
	CdbTable.cpp
	CdbTableCompare.cpp
	CdbTableColumn.cpp
	CdbTableIndex.cpp
	CdbTableIndexCompare.cpp
	ColumnIndexMatcher.cpp
	TableObjectFactory.cpp
)
handle_sub(codablecashlib "${__src}" smartcontract_db schema_table/table)

