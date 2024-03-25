/*
 * test_secp256_schnorr.cpp
 *
 *  Created on: 2022/03/23
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"


#include "ecda/ScPrivateKey.h"
#include "ecda/ScPublicKey.h"
#include "ecda/ScSignature.h"
#include "base_io/ByteBuffer.h"

#include "crypto/SchnorrKeyPair.h"

using namespace codablecash;

TEST_GROUP(TestSecp256SchnorrGroup) {
	TEST_SETUP(){}
	TEST_TEARDOWN(){}
};

TEST(TestSecp256SchnorrGroup, case01){
	ScPrivateKey privateKey;
	ScPublicKey pubKey = privateKey.generatePublicKey();

	ScSignature sig;

	const char* d = "dasdasflgjk;lfdjgk;ldfgjioviypiyvoucsormi;umguarce,@gaopcpopcxrufnkxeyfyen";
	int length = ::strlen(d);


	ByteBuffer* data = ByteBuffer::wrapWithEndian((const uint8_t*)d, length, true); __STP(data);
	sig.sign(data, privateKey.getKeyvalue());

	bool bl = sig.verify(data, pubKey);
	CHECK(bl == true);

	ScSignature* sig2 = new ScSignature(sig); __STP(sig2);
	bl = sig2->verify(data, pubKey);
	CHECK(bl == true);
}

TEST(TestSecp256SchnorrGroup, case02){
	ScPrivateKey privateKey;
	ScPublicKey pubKey = privateKey.generatePublicKey();

	int cap = pubKey.binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(cap, true); __STP(buff);
	pubKey.toBinary(buff);

	buff->position(0);
	ScPublicKey* pubKey2 = ScPublicKey::fromBinary(buff); __STP(pubKey2);

	CHECK(pubKey.equals(pubKey2));

	ScPublicKey* pubKey3 = (ScPublicKey*)pubKey2->copyData(); __STP(pubKey3);
	CHECK(pubKey.equals(pubKey3));
}

