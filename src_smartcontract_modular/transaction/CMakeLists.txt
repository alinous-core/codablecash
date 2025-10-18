

set(__src
	AbstractSmartcontractAddress.cpp
	AbstractSmartcontractTransaction.cpp
	LocalZoneSmartcontractCallTransaction.cpp
	NopSmartcontractTransaction.cpp
	RegisterSmartcontractModuleTransaction.cpp
	RemoteZoneSmartcontractCallTransaction.cpp
	SmartcontractInstanceAddress.cpp
	SmartcontractModuleAddress.cpp
)
handle_sub(codablecashlib "${__src}" smartcontract_modular transaction)

