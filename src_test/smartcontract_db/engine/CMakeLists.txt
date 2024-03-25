
set(testsrc
	DummyErrorTransactionLog.cpp
	test_cdb_value_compare.cpp
	test_get_transaction.cpp
	test_log_binary.cpp
	test_snapshot.cpp
	test_sql_engine.cpp
	test_transaction_error.cpp
	test_transaction_fw.cpp
	test_undo_misc.cpp
)
handle_sub(testall "${testsrc}" test smartcontract_db/engine)
