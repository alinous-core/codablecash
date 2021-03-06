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

TEST(SchnorrTestGroup, sign){
	SchnorrKeyPair* key = Schnorr::generateKey();

	const char* testData = "asdfghjjklqwertyuiopzxcvbnm,./poiiuuuytrtrree";
	int size = alinous::Mem::strlen(testData);

	SchnorrSignature* sig = Schnorr::sign(*key->secretKey, *key->publicKey, (const uint8_t*)testData, (size_t)size);
	bool result = Schnorr::verify(*sig, *key->publicKey, (const uint8_t*)testData, (size_t)size);

	delete key;
	delete sig;
}


