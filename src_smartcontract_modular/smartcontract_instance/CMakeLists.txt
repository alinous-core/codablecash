

set(__src
	AbstractExecutableModuleInstance.cpp
	ExecutableModuleInstance.cpp
	LibraryExectableModuleInstance.cpp
	ModularSmartcontractInstance.cpp
)
handle_sub(codablecashlib "${__src}" smartcontract_modular smartcontract_instance)

