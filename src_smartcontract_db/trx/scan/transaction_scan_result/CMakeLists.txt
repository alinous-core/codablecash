

set(__src
	GroupedScanResultMetadata.cpp
	ScanResultExecutor.cpp
	ScanResultFieldMetadata.cpp
	ScanResultMetadata.cpp
)
handle_sub(codablecashlib "${__src}" smartcontract_db trx/scan/transaction_scan_result)

