

set(__src
	Aes256Cbc.cpp
	IKeyPair.cpp
	RipeMd160.cpp
	Schnorr.cpp
	SchnorrKeyPair.cpp
	SchnorrSignature.cpp
	Sha256.cpp
	Sha256Context.cpp
)

handle_sub(codablecashlib "${__src}" null crypto)


