/*
 * test_timestamp.cpp
 *
 *  Created on: 2023/12/27
 *      Author: iizuka
 */

#include <sys/time.h>

#include "base_timestamp/SystemTimestamp.h"
#include "test_utils/t_macros.h"

TEST_GROUP(TestTimestampGroup) {
	TEST_SETUP() {}
	TEST_TEARDOWN() {}
};

TEST(TestTimestampGroup, case01){
	struct timeval myTime;

	::gettimeofday(&myTime, NULL);

	int sizeOftm = sizeof(myTime.tv_sec);
	int sizeOfutm = sizeof(myTime.tv_usec);

	int long64size = sizeof(uint64_t);

	CHECK(sizeOftm == 8);
	CHECK(sizeOfutm == 8);
}

TEST(TestTimestampGroup, case02){
	SystemTimestamp tm1;
	SystemTimestamp tm2(tm1);
}

TEST(TestTimestampGroup, case03) {
	SystemTimestamp tm1(0, 1000001);

	uint64_t sec = tm1.getSec();
	uint32_t usec = tm1.getUsec();

	CHECK(sec == 1);
	CHECK(usec == 1);
}

TEST(TestTimestampGroup, case04) {
	SystemTimestamp tm1(100, 100);
	SystemTimestamp tm2(200, 300);

	SystemTimestamp tm3 = tm1 + tm2;

	uint64_t sec = tm3.getSec();
	uint32_t usec = tm3.getUsec();

	CHECK(sec == 300);
	CHECK(usec == 400);
}

TEST(TestTimestampGroup, case05) {
	SystemTimestamp tm1(100, 100);
	SystemTimestamp tm2(200, 300);

	SystemTimestamp tm3 = tm2 - tm1;

	uint64_t sec = tm3.getSec();
	uint32_t usec = tm3.getUsec();

	CHECK(sec == 100);
	CHECK(usec == 200);
}

