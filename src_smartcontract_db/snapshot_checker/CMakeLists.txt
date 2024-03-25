

set(__src
	SnapShotDiffChecker.cpp
	SnapshotFileReader.cpp
)
handle_sub(codablecashlib "${__src}" smartcontract_db snapshot_checker)

