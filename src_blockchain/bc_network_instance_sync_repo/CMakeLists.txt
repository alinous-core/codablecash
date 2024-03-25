

set(__src
	HeightHeaderSyncDataList.cpp
	SyncHeaderDataFactory.cpp
	SyncHeaderRepo.cpp
)
handle_sub(codablecashlib "${__src}" blockchain bc_network_instance_sync_repo)

