

set(__src
	AstroBwt.cpp
	BwtTransform.cpp
	Salsa20.cpp
	Sha3256.cpp
)

handle_sub(codablecashlib "${__src}" null astrobwt)


