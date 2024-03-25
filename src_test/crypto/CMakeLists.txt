
set(testsrc
	test_aes.cpp
	test_astro_bwt.cpp
	test_btw.cpp
	test_compressed_secp256.cpp
	test_mpz.cpp
	test_musig.cpp
	test_openssl_bn_sqrt.cpp
	test_salsa20.cpp
	test_schnorr.cpp
	test_secp256_schnorr.cpp
	test_secp256.cpp
	test_sha256.cpp
	test_yescript.cpp
	test_zip.cpp
)
handle_sub(testall "${testsrc}" test crypto)

