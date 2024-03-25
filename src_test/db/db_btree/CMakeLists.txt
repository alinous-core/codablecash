
set(testsrc
	TempValue.cpp
	test_btree_memory.cpp
	test_btree_misc.cpp
	test_btree_remove_add.cpp
	test_btree_update.cpp
	test_btree.cpp
	test_cache.cpp
	test_key_format.cpp
	test_nullkey.cpp
	test_reverse_scan.cpp
)
handle_sub(testall "${testsrc}" test db/db_btree)
