

set(__src
	AbstractUndoLog.cpp
	AbstractUndoLogPart.cpp
	CdbDatabaseSessionId.cpp
	CdbDatabaseSession.cpp
	UndoIds.cpp
	UndoRegisterTransaction.cpp
	UndoSpaceManager.cpp
)
handle_sub(codablecashlib "${__src}" smartcontract_db trx/session/base)

