

set(__src
	UndoCreateTableLog.cpp
	UndoDropTableLog.cpp
	UndoIndexPart.cpp
)
handle_sub(codablecashlib "${__src}" smartcontract_db trx/session/ddl)

