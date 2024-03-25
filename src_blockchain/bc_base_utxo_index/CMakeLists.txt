

set(__src
	AddressDescriptorData.cpp
	AddressDescriptorDataFactory.cpp
	AddressDescriptorKey.cpp
	AddressDescriptorKeyFactory.cpp
	AddressDescriptorUtxoData.cpp
	AddressDescriptorUtxoDataFactory.cpp
	UtxoData.cpp
	UtxoDataFactory.cpp
	UtxoIdKey.cpp
	UtxoIdKeyFactory.cpp
)
handle_sub(codablecashlib "${__src}" blockchain bc_base_utxo_index)

