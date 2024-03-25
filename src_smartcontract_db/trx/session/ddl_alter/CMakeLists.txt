

set(__src
	UndoAlterAddColumnLog.cpp
	UndoAlterAddIndexLog.cpp
	UndoAlterAddPrimaryKeyLog.cpp
	UndoAlterDropColumnLog.cpp
	UndoAlterDropIndexLog.cpp
	UndoAlterDropPrimaryKeyLog.cpp
	UndoAlterModifyLog.cpp
	UndoAlterRenameColumnLog.cpp
	UndoAlterRenameTableLog.cpp
)
handle_sub(codablecashlib "${__src}" smartcontract_db trx/session/ddl_alter)

