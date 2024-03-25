

set(__src
	AbstractSyncWorkerThread.cpp
	AbstractZoneSync.cpp
	HeaderSyncWorker.cpp
	MemPoolSync.cpp
	MemPoolSyncWorker.cpp
	MemPoolSyncWorkerException.cpp
	NetworkNodeSync.cpp
	RamdomNodesSelector.cpp
	ZoneBlockchainSync.cpp
	ZoneBlockValidator.cpp
	ZoneHeaderSync.cpp
)
handle_sub(codablecashlib "${__src}" blockchain bc_network_instance_sync)

