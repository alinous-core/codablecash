

set(__src
	UndoLogPartsRepo.cpp
	UndoLogPartsScanner.cpp
	UndoLogsDataFactory.cpp
	UndoLogsIdkey.cpp
	UndoLogsKeyFactory.cpp
	UndoLogsPartDataFactory.cpp
	UndoLogsPartIdKey.cpp
	UndoLogsRepo.cpp
	UndoLogsRepoScanner.cpp
	UndoLogsRepoSet.cpp
)
handle_sub(codablecashlib "${__src}" smartcontract_db trx/session/repo)

