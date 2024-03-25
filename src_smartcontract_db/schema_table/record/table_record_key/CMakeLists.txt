

set(__src
	AbstractCdbKey.cpp
	CdbBooleanKey.cpp
	CdbByteKey.cpp
	CdbInfinityKey.cpp
	CdbIntKey.cpp
	CdbLongKey.cpp
	CdbNullKey.cpp
	CdbOidKey.cpp
	CdbRecordKey.cpp
	CdbShortKey.cpp
	CdbStringKey.cpp
)
handle_sub(codablecashlib "${__src}" smartcontract_db schema_table/record/table_record_key)

