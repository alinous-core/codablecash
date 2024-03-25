/*
 * test_ripemd160.cpp
 *
 *  Created on: 2023/01/23
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"

#include "base/StackRelease.h"

#include "base_io/ByteBuffer.h"

#include "crypto/RipeMd160.h"

#include "base/UnicodeString.h"

#include "numeric/BigInteger.h"

#include <cstring>

#include <cstdint>
using namespace codablecash;

TEST_GROUP(TestRipeMd160Group) {
	TEST_SETUP(){
	}
	TEST_TEARDOWN(){
	}
};

/*
RIPEMD-160 test suite results (ASCII):
* message: "" (empty string)
  hashcode: 9c1185a5c5e9fc54612808977ee8f548b2258d31
* message: "a"
  hashcode: 0bdc9d2d256b3ee9daae347be6f4dc835a467ffe
* message: "abc"
  hashcode: 8eb208f7e05d987a9b044a8e98c6b087f15a0bfc
* message: "message digest"
  hashcode: 5d0689ef49d2fae572b881b123a85ffa21595f36
* message: "abcdefghijklmnopqrstuvwxyz"
  hashcode: f71c27109c692c1b56bbdceb5b9d2865b3708dbc
* message: "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"
  hashcode: 12a053384a9c0c88e405a06c27dcf49ada62eb2b
* message: "A...Za...z0...9"
  hashcode: b0e20b6e3116640286ed3a87a5713079b21f5189
* message: 8 times "1234567890"
  hashcode: 9b752e45573d4b39f4dbd3323cab82bf63326bfb
* message: 1 million times "a"
  hashcode: 52783243c1697bdbe16d37f97f68f08325dc1528

  private let testVectors: [(String, String)] = [
    ("", "9c1185a5c5e9fc54612808977ee8f548b2258d31"),
    ("a", "0bdc9d2d256b3ee9daae347be6f4dc835a467ffe"),
    ("abc", "8eb208f7e05d987a9b044a8e98c6b087f15a0bfc"),
    ("message digest", "5d0689ef49d2fae572b881b123a85ffa21595f36"),
    ("abcdefghijklmnopqrstuvwxyz", "f71c27109c692c1b56bbdceb5b9d2865b3708dbc"),
    ("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq", "12a053384a9c0c88e405a06c27dcf49ada62eb2b"),
    ("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789", "b0e20b6e3116640286ed3a87a5713079b21f5189"),
    (String(repeating: "1234567890", count: 8), "9b752e45573d4b39f4dbd3323cab82bf63326bfb"),
    (String(repeating: "a", count: 1_000_000), "52783243c1697bdbe16d37f97f68f08325dc1528")
]
*/


static ByteBuffer* toBinaryArray(const wchar_t* vector){
	BigInteger bigint(vector, 16);
	ByteBuffer* abuff = bigint.toBinary();
	abuff->position(0);

	return abuff;
}

static int testDiff(const char* vector, const wchar_t* answer){
	ByteBuffer* ansbuff = toBinaryArray(answer); __STP(ansbuff);
	const char* answerbin = (const char*)ansbuff->array();

	int length = ::strlen(vector);
	ByteBuffer* vbuff = ByteBuffer::allocateWithEndian(length, true); __STP(vbuff);
	const uint8_t* in = (const uint8_t*)vector;
	vbuff->put(in, length);


	ByteBuffer* buff = RipeMd160::encode(vbuff); __STP(buff);

	const char* ret = (const char*)buff->array();

	int diff = Mem::memcmp(ret, answerbin, 20);
	return diff;
}

TEST(TestRipeMd160Group, case09){
	int length = 1000000;
	char* vector = new char[length + 1]{};
	StackArrayRelease<char> st_vector(vector);

	for(int i = 0; i != 1000000; ++i){
		vector[i] = 'a';
	}

	int ret =testDiff(vector, L"52783243c1697bdbe16d37f97f68f08325dc1528");
	CHECK(ret == 0);
}

TEST(TestRipeMd160Group, case08){
	int ret =testDiff("12345678901234567890123456789012345678901234567890123456789012345678901234567890", L"9b752e45573d4b39f4dbd3323cab82bf63326bfb");
	CHECK(ret == 0);
}

TEST(TestRipeMd160Group, case07){
	int ret =testDiff("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789", L"b0e20b6e3116640286ed3a87a5713079b21f5189");
	CHECK(ret == 0);
}

TEST(TestRipeMd160Group, case06){
	int ret =testDiff("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq", L"12a053384a9c0c88e405a06c27dcf49ada62eb2b");
	CHECK(ret == 0);
}

TEST(TestRipeMd160Group, case05){
	int ret =testDiff("abcdefghijklmnopqrstuvwxyz", L"f71c27109c692c1b56bbdceb5b9d2865b3708dbc");
	CHECK(ret == 0);
}

TEST(TestRipeMd160Group, case04){
	int ret =testDiff("message digest", L"5d0689ef49d2fae572b881b123a85ffa21595f36");
	CHECK(ret == 0);
}

TEST(TestRipeMd160Group, case03){
	int ret =testDiff("abc", L"8eb208f7e05d987a9b044a8e98c6b087f15a0bfc");
	CHECK(ret == 0);
}

TEST(TestRipeMd160Group, case02){
	int ret =testDiff("a", L"0bdc9d2d256b3ee9daae347be6f4dc835a467ffe");
	CHECK(ret == 0);
}


TEST(TestRipeMd160Group, case01){
	const char* vector = "";
	BigInteger bigint(L"9c1185a5c5e9fc54612808977ee8f548b2258d31", 16);
	ByteBuffer* abuff = bigint.toBinary(); __STP(abuff);
	abuff->position(0);
	const char* answer = (const char*)abuff->array();

	ByteBuffer* inbuff = ByteBuffer::allocateWithEndian(0, true); __STP(inbuff);

	ByteBuffer* buff = RipeMd160::encode(inbuff); __STP(buff);

	const char* ret = (const char*)buff->array();

	int diff = Mem::memcmp(ret, answer, 20);
	CHECK(diff == 0);
}


