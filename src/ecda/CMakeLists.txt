

set(__src
	ScPrivateKey.cpp
	ScPublicKey.cpp
	ScSignature.cpp
	Secp256k1CompressedPoint.cpp
	Secp256k1Exception.cpp
	Secp256k1Point.cpp
)

handle_sub(codablecashlib "${__src}" null ecda)


