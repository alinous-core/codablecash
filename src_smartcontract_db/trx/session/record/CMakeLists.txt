

set(__src
	UndoCreateSchemaPart.cpp
	UndoDeletedRecordColumnPart.cpp
	UndoDeleteRecordPart.cpp
	UndoInsertRecordPart.cpp
	UndoModifyRecordColumnsPart.cpp
	UndoUpdateRecordPart.cpp
	UpdatedColumn.cpp
)
handle_sub(codablecashlib "${__src}" smartcontract_db trx/session/record)

