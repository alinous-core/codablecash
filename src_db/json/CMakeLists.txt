
set(__src
	AbstractJsonToken.cpp
	IJsonStream.cpp
	JsonHandler.cpp
	JsonParseException.cpp
	JsonParser.cpp
	JsonStringStream.cpp
	JsonStringToken.cpp
	JsonToken.cpp
	JsonTokenizeException.cpp
	JsonTokenizer.cpp
	SkippedToken.cpp
	StackJsonToknizerForeseeReleaser.cpp
)
handle_sub(codablecashlib "${__src}" db json)
