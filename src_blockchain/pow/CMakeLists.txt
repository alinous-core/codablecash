
set(__src
	AbstractPoWCalculator.cpp
	AbstractPoWCommandMessage.cpp
	PoWCalculator.cpp
	PoWCalculatorThread.cpp
	PoWGenerateBlockRequestCommandMessage.cpp
	PoWManager.cpp
	PoWNonce.cpp
	PoWNonceResult.cpp
	PoWStartCalculateCommandMessage.cpp
)
handle_sub(codablecashlib "${__src}" blockchain pow)
