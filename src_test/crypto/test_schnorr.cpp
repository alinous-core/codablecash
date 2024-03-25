/*
 * test_schnorr.cpp
 *
 *  Created on: 2018/04/12
 *      Author: iizuka
 */

#include <iostream>
#include "osenv/memory.h"
#include <gmp.h>

#include "crypto/Schnorr.h"

#include "test_utils/t_macros.h"
#include "base_io/ByteBuffer.h"
#include "crypto/Sha256.h"
#include "yescrypt/sha256.h"
#include "base/Exception.h"

#include "../../src/crypto/SchnorrSignature.h"
using namespace codablecash;
using namespace alinous;

TEST_GROUP(SchnorrTestGroup) {
	TEST_SETUP() {}
	TEST_TEARDOWN() {}

};

TEST(SchnorrTestGroup, IKeyPairCreateError){
	Exception* ex = nullptr;

	try{
		ByteBuffer* buff = ByteBuffer::allocateWithEndian(32, true); __STP(buff);
		buff->put(128);
		buff->position(0);

		IKeyPair::createFromBinary(buff);
	}
	catch (Exception* e) {
		ex = e;
	}

	CHECK(ex != nullptr)
	delete ex;

}

TEST(SchnorrTestGroup, sha){
	ByteBuffer* bin = Schnorr::toByteBuffer(Schnorr::cnsts.G);

	for(int i = 0; i != 10; ++i){
		SHA256_CTX ctx;
		SHA256_Init(&ctx);
		SHA256_Update(&ctx, bin->array(), bin->capacity());

		uint8_t sha256[32];
		SHA256_Final((uint8_t *)sha256, &ctx);
	}

	delete bin;
}

TEST(SchnorrTestGroup, test01){
	Schnorr *psc = new Schnorr();

	delete psc;
}

TEST(SchnorrTestGroup, generateKey){
	SchnorrKeyPair* key = Schnorr::generateKey();

	BigInteger ans = Schnorr::cnsts.G.modPow(*key->secretKey, Schnorr::cnsts.Q);
	CHECK(ans.equals(key->publicKey));

	delete key;
}

/*
 *
 * Failure Report
  SchnorrTestGroup
    sign(/ramdisk/codablecash/src_test/crypto/test_schnorr.cpp at 79)
      Check at 88
Testing Duration : 9475.234 ms
 *
 */

TEST(SchnorrTestGroup, sign){
	for(int i = 0; i != 10; ++i)
	{
		SchnorrKeyPair* key = Schnorr::generateKey();

		const char* testData = "asdfghjjklqwertyuiopzxcvbnm,./poiiuuuytrtrree";
		int size = alinous::Mem::strlen(testData);

		SchnorrSignature* sig = Schnorr::sign(*key->secretKey, *key->publicKey, (const uint8_t*)testData, (size_t)size);
		bool result = Schnorr::verify(*sig, *key->publicKey, (const uint8_t*)testData, (size_t)size);

		CHECK(result == true);

		delete key;
		delete sig;
	}

}

TEST(SchnorrTestGroup, sign2){
	SchnorrKeyPair* key = Schnorr::generateKey();

	const char* testData = "asdfghjjklqwertyuiopzxcvbnm,./poiiuuuytrtrree";
	int size = alinous::Mem::strlen(testData);

	ByteBuffer* buffData = ByteBuffer::wrapWithEndian(testData, size, true); __STP(buffData);

	SchnorrSignature* sig = Schnorr::sign(*key->secretKey, *key->publicKey, buffData);
	bool result = Schnorr::verify(*sig, *key->publicKey, buffData);

	CHECK(result == true);

	{
		int capacity = sig->binarySize();
		ByteBuffer* buff = ByteBuffer::allocateWithEndian(capacity, true); __STP(buff);
		sig->toBinary(buff);

		buff->position(0);

		SchnorrSignature* sig2 = SchnorrSignature::createFromBinary(buff); __STP(sig2);
		result = Schnorr::verify(*sig2, *key->publicKey, buffData);
		CHECK(result == true);
	}

	delete key;
	delete sig;
}

TEST(SchnorrTestGroup, copyConstractor){
	BigInteger e(100), y(200);
	SchnorrSignature sig(e, y);
	SchnorrSignature sig2(sig);

	CHECK(sig.e->equals(sig2.e));
	CHECK(sig.y->equals(sig2.y));
}

TEST(SchnorrTestGroup, keypair01){
	SchnorrKeyPair* key = Schnorr::generateKey(); __STP(key);
	SchnorrKeyPair* key2 = dynamic_cast<SchnorrKeyPair*>(key->clone()); __STP(key2);

	SchnorrKeyPair* key3 = nullptr;
	{
		int cap = key2->binarySize();
		ByteBuffer* buff = ByteBuffer::allocateWithEndian(cap, true); __STP(buff);
		key2->toBinary(buff);

		buff->position(0);
		key3 = dynamic_cast<SchnorrKeyPair*>(SchnorrKeyPair::createFromBinary(buff));
	}
	__STP(key3);

	CHECK(key3->getPubKey()->equals(key->getPubKey()));
	CHECK(key3->getSecretKey() == nullptr);
}

