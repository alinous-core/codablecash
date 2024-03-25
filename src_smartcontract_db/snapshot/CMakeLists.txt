

set(__src
	AbstractSnapshotCommand.cpp
	FileSnapshotRecoverer.cpp
	FileStoreSnapshot.cpp
	IdsBackupCommand.cpp
	ISnapshotHandler.cpp
	ISnapshotRecoverer.cpp
	SchemaEndSnapshotCommand.cpp
	SchemaStartSnapshotCommand.cpp
	SnapshotIndexCommand.cpp
	SnapshotRecordCommand.cpp
	SnapShotSha256.cpp
	TableEndSnapshotCommand.cpp
	TableStartSnapshotCommand.cpp
)
handle_sub(codablecashlib "${__src}" smartcontract_db snapshot)

