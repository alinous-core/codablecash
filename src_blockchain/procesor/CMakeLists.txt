
set(__src
	BlankCommandParameter.cpp
	ICommandMessage.cpp
	ICommandParameter.cpp
	MessageProcessor.cpp
	MessageProcessorThread.cpp
	NopCommandMessage.cpp
)
handle_sub(codablecashlib "${__src}" blockchain procesor)
