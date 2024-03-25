
set(__src
	AbstractContainerHashAlgorithmElement.cpp
	AbstractHashAlgorithmElement.cpp
	PoWAesAlgo.cpp
	PoWAstroBwtAlgo.cpp
	PoWForLoop.cpp
	PoWHashProgram.cpp
	PowRandomHashManager.cpp
	PoWSalsaAlgo.cpp
	PoWSha256Algo.cpp
	RandomShaker.cpp
	TotalNumberSplitter.cpp
)
handle_sub(codablecashlib "${__src}" blockchain pow_hash)
