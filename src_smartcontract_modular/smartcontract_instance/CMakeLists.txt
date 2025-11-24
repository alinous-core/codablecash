

set(__src
	AbstractExecutableModuleInstance.cpp
	ExecutableModuleInstance.cpp
	InstanceDependencyContext.cpp
	InstanceDependencyHandler.cpp
	LibraryExectableModuleInstance.cpp
	ModularSmartcontractInstance.cpp
	ModuleInstanceClassLoader.cpp
)
handle_sub(codablecashlib "${__src}" smartcontract_modular smartcontract_instance)

