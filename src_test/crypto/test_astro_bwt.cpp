/*
 * test_astro_bwt.cpp
 *
 *  Created on: 2022/04/20
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"

#include "astrobwt/AstroBwt.h"

#include "base/StackRelease.h"

#include "base_io/ByteBuffer.h"

using namespace alinous;
using namespace codablecash;

TEST_GROUP(TestAstroBtwGroup) {
	TEST_SETUP(){}
	TEST_TEARDOWN(){}
};

TEST(TestAstroBtwGroup, case01){
	const char* data = "hello world aaaa";
	int length = ::strlen(data);

	AstroBwt astro;

	for(int i = 0; i != 1; ++i){
		ByteBuffer* buff = astro.POW((const uint8_t*)data, length); __STP(buff);
	}

	uint16_t shortValue = 0xff;
	char* ptr = (char*)&shortValue;
	shortValue = shortValue << 8;

	uint8_t low = ptr[0];
	uint8_t high = ptr[1];
}
