

set(__src
	InstanceIdIndex.cpp
	InstanceIdIndexData.cpp
	InstanceIdIndexDataFactory.cpp
	InstanceIdIndexKey.cpp
	InstanceIdIndexKeyFactory.cpp
)
handle_sub(codablecashlib "${__src}" smartcontract_modular smartcontract_executor_index)

