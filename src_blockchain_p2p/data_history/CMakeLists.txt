

set(__src
	AbstractTransferedData.cpp
	DataHistory.cpp
	DataHistoryStore.cpp
	TimeIndexKey.cpp
	TimeIndexKeyFactory.cpp
	TransferedDataFactory.cpp
	TransferedDataId.cpp
	TransferedDataIdKey.cpp
	TransferedDataIdKeyFactory.cpp
	TransferedDataIdList.cpp
	TransferedDataIdListFactory.cpp
)
handle_sub(codablecashlib "${__src}" blockchain_p2p data_history)

