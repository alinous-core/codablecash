

set(__src
	UndoAddIndexPart.cpp
	UndoIndexPrimaryPart.cpp
	UndoIndexUniquePart.cpp
	UndoRemoveIndexPart.cpp
)
handle_sub(codablecashlib "${__src}" smartcontract_db trx/session/index)

