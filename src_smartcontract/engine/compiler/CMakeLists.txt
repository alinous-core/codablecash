

set(__src
	CompileError.cpp
	LexError.cpp
	LexErrorHandler.cpp
	ParseErrorHandler.cpp
	ParserReaderStream.cpp
	SmartContractParser.cpp
	StatementRepairSkipper.cpp
)
handle_sub(codablecashlib "${__src}" smartcontract engine/compiler)

