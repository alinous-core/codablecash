/*
 * test_wallet_encoder.cpp
 *
 *  Created on: 2023/02/22
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"

#include "bc_wallet/HdWalletSeed.h"

#include "bc_wallet_encoder/PasswordEncoder.h"

#include "bc_base/Base64.h"

#include "crypto/Sha256.h"
using namespace codablecash;

TEST_GROUP(TestWalletEncoderGroup) {
	TEST_SETUP(){}
	TEST_TEARDOWN(){}
};

TEST(TestWalletEncoderGroup, case01){
	UnicodeString pass(L"changeit");
	HdWalletSeed* seed = HdWalletSeed::newSeed(); __STP(seed);

	PasswordEncoder encoder(&pass);

	HdWalletSeed* encoded = encoder.encode(seed); __STP(encoded);
	HdWalletSeed* decoded = encoder.decode(encoded); __STP(decoded);

	bool bl = seed->equals(decoded);
	CHECK(bl);
}

TEST(TestWalletEncoderGroup, base64_01){
	HdWalletSeed* seed = HdWalletSeed::newSeed(); __STP(seed);
	int size = seed->size();
	const char* data = seed->toArray();

	UnicodeString* str = Base64::encode((const uint8_t*)data, size); __STP(str);

	const char* base64 = str->toCString();
	StackArrayRelease<const char> __st_base64(base64);
	int len = ::strlen(base64);
	ByteBuffer* buff = Base64::decode(base64, len); __STP(buff);

	int bsize = buff->limit();
	const char* bdata = (const char*)buff->array();

	CHECK(size == bsize);
	int diff = Mem::memcmp(data, bdata, size);
	CHECK(diff == 0);
}

TEST(TestWalletEncoderGroup, base64_02){
	HdWalletSeed* seed = HdWalletSeed::newSeed(); __STP(seed);

	ByteBuffer* sh= Sha256::sha256(seed->getByteBuffer(), true); __STP(sh);
	int size = sh->limit();
	const char* data = (const char*)sh->array();

	UnicodeString* str = Base64::encode((const uint8_t*)data, size); __STP(str);

	const char* base64 = str->toCString();
	StackArrayRelease<const char> __st_base64(base64);
	int len = ::strlen(base64);
	ByteBuffer* buff = Base64::decode(base64, len); __STP(buff);

	int bsize = buff->limit();
	const char* bdata = (const char*)buff->array();

	CHECK(size == bsize);
	int diff = Mem::memcmp(data, bdata, size);
	CHECK(diff == 0);
}

TEST(TestWalletEncoderGroup, base64_03){
	HdWalletSeed* seed = HdWalletSeed::newSeed(); __STP(seed);

	ByteBuffer* sh= Sha256::sha256(seed->getByteBuffer(), true); __STP(sh);

	int capacity = seed->size() + sh->limit();
	ByteBuffer* input = ByteBuffer::allocateWithEndian(capacity, true); __STP(input);

	input->put((const uint8_t*)seed->toArray(), seed->size());
	input->put(sh);

	int size = input->limit();
	const char* data = (const char*)input->array();

	UnicodeString* str = Base64::encode((const uint8_t*)data, size); __STP(str);

	const char* base64 = str->toCString();
	StackArrayRelease<const char> __st_base64(base64);
	int len = ::strlen(base64);
	ByteBuffer* buff = Base64::decode(base64, len); __STP(buff);

	int bsize = buff->limit();
	const char* bdata = (const char*)buff->array();

	CHECK(size == bsize);
	int diff = Mem::memcmp(data, bdata, size);
	CHECK(diff == 0);

}
