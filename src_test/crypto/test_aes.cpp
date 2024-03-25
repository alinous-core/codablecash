/*
 * test_aes.cpp
 *
 *  Created on: 2023/01/27
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"

#include "aes256cbc/aes256.h"

#include "crypto/Aes256Cbc.h"

#include "base/UnicodeString.h"
using namespace codablecash;

static const uint8_t IV[16]={0x86, 0xaf, 0xc4, 0x38, 0x68, 0xfe, 0xa6, 0xab, 0xd4, 0x0f, 0xbf, 0x6d, 0x5e, 0xd5, 0x09, 0x05};
static const uint8_t Key256[32]={0xf4, 0x15, 0x0d, 0x4a, 0x1a, 0xc5, 0x70, 0x8c, 0x29, 0xe4, 0x37, 0x74, 0x90, 0x45, 0xa3, 0x9a, 0x29, 0xe4, 0x37, 0x74, 0x1a, 0xc5, 0x70, 0x8c, 0xf4, 0x15, 0x0d, 0x4a, 0x90, 0x45, 0xa3, 0x9a};

TEST_GROUP(TestAESGroup) {
	TEST_SETUP(){}
	TEST_TEARDOWN(){}
};

TEST(TestAESGroup, case01){
	aes256_context ctx;
	uint8_t buf[16] = "Hello World!";
	uint8_t buf_before[16] = "Hello World!";

	aes256_init_ex(&ctx, Key256, IV);
	aes256_encrypt_ecb(&ctx, buf);

	aes256_init_ex(&ctx, Key256, IV);
	aes256_decrypt_ecb(&ctx, buf);

	int diff = Mem::memcmp(buf, buf_before, 16);
	CHECK(diff == 0);

	aes256_done(&ctx);
}

TEST(TestAESGroup, case02){
	aes256_context ctx;
	uint8_t buf[16] = "Hello World!"; // 1 block
	uint8_t buf_before[16] = "Hello World!";

	aes256_init_ex(&ctx, Key256, IV);
	aes256_encrypt_cbc(&ctx, buf);

	aes256_init_ex(&ctx, Key256, IV);
	aes256_decrypt_cbc(&ctx, buf);

	int diff = Mem::memcmp(buf, buf_before, 16);
	CHECK(diff == 0);

	aes256_done(&ctx);
}

TEST(TestAESGroup, case03){
	aes256_context ctx;
	uint8_t buf[16] = "Hello World!";
	uint8_t buf_before[16] = "Hello World!";

	aes256_init(&ctx, Key256);
	aes256_encrypt_ecb(&ctx, buf);

	aes256_init(&ctx, Key256);
	aes256_decrypt_ecb(&ctx, buf);

	int diff = Mem::memcmp(buf, buf_before, 16);
	CHECK(diff == 0);

	aes256_done(&ctx);
}

TEST(TestAESGroup, case04){
	Aes256Cbc aes;

	uint8_t buf[16] = "Hello World!";
	UnicodeString str((const char*)buf);

	Aes256CbcResult* encrypted = aes.encryptoPlainText(&str); __STP(encrypted);

	const char* ret = aes.decrypt(encrypted->data, encrypted->length);
	int diff = Mem::memcmp(buf, ret, 13);
	CHECK(diff == 0);

	delete [] ret;
}

TEST(TestAESGroup, case05){
	Aes256Cbc aes;

	uint8_t buf[17] = "Hello World!1234";
	UnicodeString str((const char*)buf);

	Aes256CbcResult* encrypted = aes.encryptoPlainText(&str); __STP(encrypted);

	const char* ret = aes.decrypt(encrypted->data, encrypted->length);
	int diff = Mem::memcmp(buf, ret, 16);
	CHECK(diff == 0);

	delete [] ret;
}

TEST(TestAESGroup, case06){
	Aes256Cbc aes;
	aes.setIv(Aes256Cbc::DEFAULT_IV, 16);
	aes.setKey(Aes256Cbc::DEFAULT_Key256);

	const char* buf = "Hello World! Hello World! Hello World! Hello World!";
	UnicodeString str((const char*)buf);

	Aes256CbcResult* encrypted = aes.encryptoPlainText(&str); __STP(encrypted);

	const char* ret = aes.decrypt(encrypted->data, encrypted->length);
	int diff = Mem::memcmp(buf, ret, 13);
	CHECK(diff == 0);

	delete [] ret;
}

TEST(TestAESGroup, error01){
	Aes256Cbc aes;
	const char* ret = aes.decrypt(nullptr, 3);
	CHECK(ret == nullptr);
}
