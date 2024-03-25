

set(__src
	DatabaseEqualsChecker.cpp
	DatabaseEqualsCheckException.cpp
	ISchemaUptateListner.cpp
	Schema.cpp
	SchemaAlterCommandsHandler.cpp
	SchemaCompare.cpp
	SchemaManager.cpp
	SchemaRoot.cpp
)
handle_sub(codablecashlib "${__src}" smartcontract_db schema_table/schema)

