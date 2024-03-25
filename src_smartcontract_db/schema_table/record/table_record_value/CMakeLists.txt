

set(__src
	AbstractCdbValue.cpp
	CdbBooleanValue.cpp
	CdbByteValue.cpp
	CdbInfinityValue.cpp
	CdbIntValue.cpp
	CdbLongValue.cpp
	CdbNullValue.cpp
	CdbOidValueList.cpp
	CdbOidValueListCursor.cpp
	CdbShortValue.cpp
	CdbStringValue.cpp
	CdbValueCaster.cpp
	CdbValueCastException.cpp
	CdbValueList.cpp;
	VmInstanceValueConverter.cpp
)
handle_sub(codablecashlib "${__src}" smartcontract_db schema_table/record/table_record_value)

