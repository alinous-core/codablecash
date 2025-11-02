/*
 * test_bloomfilter512_value.cpp
 *
 *  Created on: May 6, 2025
 *      Author: iizuka
 */
#include "test_utils/t_macros.h"

#include "base/StackRelease.h"

#include "base_io/ByteBuffer.h"
#include "bc_wallet_filter/BloomHash1024.h"

using namespace codablecash;

TEST_GROUP(TestBloomFilter1024ValueGroup) {
	TEST_SETUP(){
	}
	TEST_TEARDOWN(){
	}
};

TEST(TestBloomFilter1024ValueGroup, case01){
	uint16_t values[3]{277, 301, 5};
	BloomHash1024 val(values[0], values[1], values[2]);

	uint16_t v0 = val.getValue(0);
	uint16_t v1 = val.getValue(1);
	uint16_t v2 = val.getValue(2);

	CHECK(v0 == values[0]);
	CHECK(v1 == values[1]);
	CHECK(v2 == values[2]);
}

TEST(TestBloomFilter1024ValueGroup, case02){
	uint16_t values[3]{300, 400, 256};
	BloomHash1024 val(values[0], values[1], values[2]);

	uint16_t v0 = val.getValue(0);
	uint16_t v1 = val.getValue(1);
	uint16_t v2 = val.getValue(2);

	CHECK(v0 == values[0]);
	CHECK(v1 == values[1]);
	CHECK(v2 == values[2]);
}

TEST(TestBloomFilter1024ValueGroup, case03){
	uint16_t values[3]{277, 301, 5};
	BloomHash1024 val(values[0], values[1], values[2]);

	int cap = val.binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(cap, true); __STP(buff);
	val.toBinary(buff);

	buff->position(0);

	BloomHash1024* newVal = BloomHash1024::createFromBinary(buff); __STP(newVal);
	bool bl = newVal->equals(&val);

	CHECK(bl == true);
}

TEST(TestBloomFilter1024ValueGroup, case04){
	uint16_t values[3]{1023, 1023, 1023};
	BloomHash1024 val(values[0], values[1], values[2]);

	uint16_t v0 = val.getValue(0);
	uint16_t v1 = val.getValue(1);
	uint16_t v2 = val.getValue(2);

	CHECK(v0 == values[0]);
	CHECK(v1 == values[1]);
	CHECK(v2 == values[2]);
}

TEST(TestBloomFilter1024ValueGroup, case05){
	uint16_t values[3]{4929, 769, 4171};
	BloomHash1024 val(values[0], values[1], values[2]);

	uint16_t v0 = val.getValue(0);
	uint16_t v1 = val.getValue(1);
	uint16_t v2 = val.getValue(2);

	CHECK(v0 == values[0]);
	CHECK(v1 == values[1]);
	CHECK(v2 == values[2]);
}


