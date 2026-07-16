set(__src
	SystemTimestamp.cpp
	SystemTimestampKey.cpp
	SystemTimestampKeyFactory.cpp
)

handle_sub(codablecashlib "${__src}" db base_timestamp)

