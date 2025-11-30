

set(__src
	InstanceSpace.cpp
	InstanceSpacesManager.cpp
	MolularSmartcontractCacheManager.cpp
	ProjectNotFoundException.cpp
)
handle_sub(codablecashlib "${__src}" smartcontract_modular smartcontract_cache)

