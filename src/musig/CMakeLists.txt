

set(__src
	IMuSigSigner.cpp
	MuSig.cpp
	MuSigBuilder.cpp
	MuSigHashBuilder.cpp
	SimpleMuSigSigner.cpp
)

handle_sub(codablecashlib "${__src}" null musig)


