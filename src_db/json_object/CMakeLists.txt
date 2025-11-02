
set(__src
	AbstractJsonContainer.cpp
	AbstractJsonObject.cpp
	AbstractJsonValue.cpp
	JsonArrayObject.cpp
	JsonBooleanValue.cpp
	JsonNumericValue.cpp
	JsonObject.cpp
	JsonStringValue.cpp
	JsonValuePair.cpp
)
handle_sub(codablecashlib "${__src}" db json_object)
