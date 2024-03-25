/*
 * test_salsa20.cpp
 *
 *  Created on: 2022/04/26
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"
#include "astrobwt/Salsa20.h"
#include <cstdint>

#include "osenv/memory.h"
using namespace alinous;
using namespace codablecash;

TEST_GROUP(TestSalsa20Group) {
	TEST_SETUP(){}
	TEST_TEARDOWN(){}
};

TEST(TestSalsa20Group, case01){
	uint8_t message[64]{};
	uint8_t res[64]{};
	uint8_t expected[64]{};

	uint8_t IV[8]{101, 102, 103, 104, 105, 106, 107, 108};
	uint8_t	key[32] { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 } ;

	Salsa20::crypto_stream_xor(res, message, 64, IV, 0, key);

	Salsa20::crypto_stream_xor(res, res, 64, IV, 0, key);

	int cmp = Mem::memcmp(res, expected, 64);
	CHECK(cmp == 0);
}
