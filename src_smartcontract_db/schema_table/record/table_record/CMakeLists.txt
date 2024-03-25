

set(__src
	CdbDataFactory.cpp
	CdbGroupedRecord.cpp
	CdbKeyFactory.cpp
	CdbRecord.cpp
	CdbTableIdentifier.cpp
	ScanRecordIndentity.cpp
	ScanRecordIndentityKey.cpp
)
handle_sub(codablecashlib "${__src}" smartcontract_db schema_table/record/table_record)

