

set(__src
	AbstractSmartcontractModule.cpp
	ExecutableSmartcontractModule.cpp
	LibrarySmartcontractModule.cpp
	ModularSmartcontractProject.cpp
)
handle_sub(codablecashlib "${__src}" smartcontract_modular modular_project)

