/*
 * test_software_version.cpp
 *
 *  Created on: 2023/01/25
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"

#include "bc/SoftwareVersion.h"

using namespace codablecash;

TEST_GROUP(TestSoftwareVersionGroup) {
	TEST_SETUP(){}
	TEST_TEARDOWN(){}
};

TEST(TestSoftwareVersionGroup, case01){
	SoftwareVersion* version = new SoftwareVersion(1, 2, 5); __STP(version);
	SoftwareVersion* version2 = new SoftwareVersion(1, 3, 5); __STP(version2);

	uint64_t lv = version->toLongValue();
	uint64_t lv2 = version2->toLongValue();

	CHECK(lv < lv2);
}

