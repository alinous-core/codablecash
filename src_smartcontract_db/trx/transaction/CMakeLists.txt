

set(__src
	CdbTransaction.cpp
	CdbTransactionManager.cpp
	LocalOid2PublishedOidLog.cpp
	RecordObjectIdPublisher.cpp
	SchemaObjectIdPublisher.cpp
)
handle_sub(codablecashlib "${__src}" smartcontract_db trx/transaction)

