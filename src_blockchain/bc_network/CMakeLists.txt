

set(__src
	NodeIdentifier.cpp
	NodeIdentifierKey.cpp
	NodeIdentifierKeyFactory.cpp
	NodeIdentifierSource.cpp
)
handle_sub(codablecashlib "${__src}" blockchain bc_network)

