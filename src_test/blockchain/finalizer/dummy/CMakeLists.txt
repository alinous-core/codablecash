
set(testsrc
	BlankBlockGenerator.cpp
	BlankBlockGeneratorAllocator.cpp
	BlankBlockWithCoinbaseGenerator.cpp
	BlankBlockWithCoinbaseGenerator02.cpp
	FinalizeMissAndBlankblockTestAllocator.cpp
	FinalizeMissDetectTicketCommandMessage.cpp
	FinalizeMissTestAllocator.cpp
	FinalizeMissTestAllocator02.cpp
	FinalizeMissTestPool.cpp
)
handle_sub(testall "${testsrc}" test blockchain/finalizer/dummy)

