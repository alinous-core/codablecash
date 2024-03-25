/*
 * test_compressed_secp256.cpp
 *
 *  Created on: 2023/02/03
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"
#include "numeric/BigInteger.h"

#include "ecda/Secp256k1Point.h"
#include "ecda/Secp256k1CompressedPoint.h"
#include "ecda/Secp256k1Exception.h"

#include "base/StackRelease.h"

#include "base_io/ByteBuffer.h"

using namespace codablecash;

TEST_GROUP(TestCompressedSecp256Group) {
	TEST_SETUP(){}
	TEST_TEARDOWN(){}
};

TEST(TestCompressedSecp256Group, case01){
	Secp256k1Point pt;

	Secp256k1CompressedPoint dpt = Secp256k1CompressedPoint::compress(&pt);

	Secp256k1Point pt2 = dpt.decompress();
	bool bl = pt.equals(pt2);
	CHECK(bl);

	Secp256k1Point pt3 = pt2;
	bl = pt.equals(pt3);
	CHECK(bl);

	Secp256k1CompressedPoint* dpt2 = dynamic_cast<Secp256k1CompressedPoint*>(dpt.copyData()); __STP(dpt2);
	dpt = *dpt2;
	pt2 = dpt.decompress();
	bl = pt.equals(pt2);
	CHECK(bl);
}

TEST(TestCompressedSecp256Group, case02){
	Secp256k1Point pt(BigInteger(0L), BigInteger(0L));

	Secp256k1Exception* ex = nullptr;
	try{
		Secp256k1CompressedPoint dpt = Secp256k1CompressedPoint::compress(&pt);
		Secp256k1Point pt2 = dpt.decompress();
	}
	catch(Secp256k1Exception* e){
		ex = e;
	}
	CHECK(ex != nullptr);
	delete ex;
}

TEST(TestCompressedSecp256Group, case03){
	Secp256k1Point pt;

	Secp256k1CompressedPoint dpt = Secp256k1CompressedPoint::compress(&pt);

	int cap = dpt.binarySize();
	ByteBuffer *buff = ByteBuffer::allocateWithEndian(cap, true); __STP(buff);
	dpt.toBinary(buff);

	buff->position(0);

	Secp256k1CompressedPoint* dpt2 = Secp256k1CompressedPoint::fromBinary(buff); __STP(dpt2);
	Secp256k1Point pt2 = dpt2->decompress();
	bool bl = pt.equals(pt2);
	CHECK(bl);
}

TEST(TestCompressedSecp256Group, testEx01){
	testException<Secp256k1Exception>();
}
