

set(__src
	AbstractUndoDmlLog.cpp
	UndoDeleteLog.cpp
	UndoInsertLog.cpp
	UndoUpdateLog.cpp
)
handle_sub(codablecashlib "${__src}" smartcontract_db trx/session/dml)

