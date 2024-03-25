

set(__src
	AbstractConfigStoreElement.cpp
	BinaryValueConfigStoreValue.cpp
	LongValueConfigStoreValue.cpp
	ShortValueConfigStoreValue.cpp
	StatusStore.cpp
)
handle_sub(codablecashlib "${__src}" blockchain bc_base_conf_store)

