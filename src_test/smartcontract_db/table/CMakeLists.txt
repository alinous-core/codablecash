
set(testsrc
	test_binary_object_check_error.cpp
	test_cdb_key_values.cpp
	test_cdb_values_cast_default.cpp
	test_cdb_values_cast.cpp
	test_cdb_values.cpp
	test_insert_parts_types.cpp
	test_insert_parts.cpp
	test_insert.cpp
	test_metadata.cpp
	test_scan_part.cpp
	test_schema.cpp
	test_value_converter.cpp
)
handle_sub(testall "${testsrc}" test smartcontract_db/table)
