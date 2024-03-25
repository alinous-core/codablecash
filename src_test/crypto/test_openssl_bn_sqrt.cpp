/*
 * test_openssl_bn_sqrt.cpp
 *
 *  Created on: 2023/01/31
 *      Author: iizuka
 */
#include <iostream>
#include <gmp.h>

#include "mod_sqrt/bn_sqrt.h"
#include "test_utils/t_macros.h"

using namespace alinous;

TEST_GROUP(OpenSSLSqrtTestGroup) {
	TEST_SETUP(){}
	TEST_TEARDOWN(){}

};

/**
 * static const BigInteger p; // = new BigInteger("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F", 16);
 *
 *　02 = positive
 *　03 = negative
 *
 * G = 02 79BE667E F9DCBBAC 55A06295 CE870B07 029BFCDB 2DCE28D9 59F2815B 16F81798
 * G = 04 79BE667E F9DCBBAC 55A06295 CE870B07 029BFCDB 2DCE28D9 59F2815B 16F81798 483ADA77 26A3C465 5DA4FBFC 0E1108A8 FD17B448 A6855419 9C47D08F FB10D4B8
 *        483ADA77 26A3C465 5DA4FBFC 0E1108A8 FD17B448 A6855419 9C47D08F FB10D4B8
 */
TEST(OpenSSLSqrtTestGroup, case03){
	mpz_t result, a, p, vec, tmp;

	mpz_init(result);
	mpz_init_set_str(a, "79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798", 16);
	mpz_init_set_str(p, "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F", 16);

	mpz_init_set_str(vec, "483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8", 16);
	mpz_init(tmp);

	int ret = BN_mod_sqrt(result, a, p);
	CHECK(ret == 0);

	// in^2
	mpz_powm_ui(tmp, result, 2, p);

	int diff = mpz_cmp(tmp, a);
	CHECK(diff == 0);

	//mpz_get_str (char *str, int base, mpz_t op)
	{
		char outstr[1024]{};
		mpz_get_str(outstr, 16, result);
	}
	char outstr[1024]{};
	mpz_get_str(outstr, 16, result);

	// check vec
	{
		// y^{2} = x^{3} + 7
		mpz_powm_ui(tmp, a, 3, p);
		mpz_add_ui(tmp, tmp, 7);

		ret = BN_mod_sqrt(result, tmp, p);
		CHECK(diff == 0);

		char outstr2[1024]{};
		mpz_get_str(outstr, 16, result);

		diff = mpz_cmp(result, vec);
		CHECK(diff == 0);
	}

	mpz_clear(result);
	mpz_clear(a);
	mpz_clear(p);
	mpz_clear(vec);
	mpz_clear(tmp);
}


/*
print(square_root(67, 73))
# => "33" or "40"
print(square_root(4, 37))
# => "2" or "35"
print(square_root(6, 37))
# => "-1"
*/
TEST(OpenSSLSqrtTestGroup, case02){
	mpz_t in, a, p;

	mpz_init(in);
	mpz_init_set_ui(a, 4);
	mpz_init_set_ui(p, 37);

	int ret = BN_mod_sqrt(in, a, p);
	CHECK(ret == 0);

	int64_t result = mpz_get_si(in);
	CHECK(result == 2);

	mpz_clear(in);
	mpz_clear(a);
	mpz_clear(p);
}

TEST(OpenSSLSqrtTestGroup, case01){
	mpz_t in, a, p;

	mpz_init(in);
	mpz_init_set_ui(a, 67);
	mpz_init_set_ui(p, 73);

	int ret = BN_mod_sqrt(in, a, p);
	CHECK(ret == 0);

	int64_t result = mpz_get_si(in);
	CHECK(result == 33);

	mpz_clear(in);
	mpz_clear(a);
	mpz_clear(p);
}



