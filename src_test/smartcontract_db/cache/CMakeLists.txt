
set(testsrc
	test_delete_trx_cache.cpp
	test_local_oid.cpp
	test_oid_array_cache.cpp
	test_oid_cache.cpp
	test_oid_key.cpp
	test_record_cache.cpp
	test_record_identity_cache.cpp
	test_record_identity.cpp
	test_update_trx_cache.cpp
)
handle_sub(testall "${testsrc}" test smartcontract_db/cache)
