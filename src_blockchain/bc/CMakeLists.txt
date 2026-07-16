

set(__src
	BlockchainSoftwareVersion.cpp
	CodablecashNodeInstance.cpp
	CodablecashSystemParam.cpp
	DebugDefaultLogger.cpp
	DefaultInstanceMemberAllocator.cpp
	exceptions.cpp
	IInstanceMemberAllocator.cpp
	ISystemLogger.cpp
	SoftwareVersion.cpp
)
handle_sub(codablecashlib "${__src}" blockchain bc)

