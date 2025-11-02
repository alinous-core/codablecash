/*
 * test_modular_version.cpp
 *
 *  Created on: Oct 4, 2025
 *      Author: iizuka
 */
#include "test_utils/t_macros.h"

#include "modular_project/ModularSmartcontractVersion.h"

using namespace codablecash;

TEST_GROUP(TestModularVersionGroup) {
	TEST_SETUP() {
		env->setup();
	}
	TEST_TEARDOWN() {
		env->teardown();
	}
};

TEST(TestModularVersionGroup, caret01){
	ModularSmartcontractVersion ver1_1_1(1, 1, 1, ModularSmartcontractVersion::TYPE_CARET);
	ModularSmartcontractVersion ver1_2_1(1, 2, 1, ModularSmartcontractVersion::TYPE_CARET);
	ModularSmartcontractVersion ver0_1_1(0, 1, 1, ModularSmartcontractVersion::TYPE_CARET);
	ModularSmartcontractVersion ver1_0_1(1, 0, 1, ModularSmartcontractVersion::TYPE_CARET);
	ModularSmartcontractVersion ver2_0_0(2, 0, 0, ModularSmartcontractVersion::TYPE_CARET);

	bool bl = ver1_1_1.validate(&ver0_1_1);
	CHECK(bl == false);

	bl = ver1_1_1.validate(&ver1_0_1);
	CHECK(bl == false);

	bl = ver1_1_1.validate(&ver2_0_0);
	CHECK(bl == false);

	bl = ver1_1_1.validate(&ver1_1_1);
	CHECK(bl == true);

	bl = ver1_1_1.validate(&ver1_2_1);
	CHECK(bl == true);
}

TEST(TestModularVersionGroup, caret02){
	ModularSmartcontractVersion ver1_1_1(1, 1, 1, ModularSmartcontractVersion::TYPE_CARET);
	ModularSmartcontractVersion ver0_1_1(0, 1, 1, ModularSmartcontractVersion::TYPE_CARET);
	ModularSmartcontractVersion ver0_0_1(0, 0, 1, ModularSmartcontractVersion::TYPE_CARET);

	bool bl = ver0_1_1.validate(&ver0_1_1);
	CHECK(bl == true);
}

TEST(TestModularVersionGroup, caret03){
	ModularSmartcontractVersion ver1_1_1(1, 1, 1, ModularSmartcontractVersion::TYPE_CARET);
	ModularSmartcontractVersion ver0_1_1(0, 1, 1, ModularSmartcontractVersion::TYPE_CARET);
	ModularSmartcontractVersion ver0_0_1(0, 0, 1, ModularSmartcontractVersion::TYPE_CARET);

	bool bl = ver0_0_1.validate(&ver0_0_1);
	CHECK(bl == true);
}

TEST(TestModularVersionGroup, tilde03){
	ModularSmartcontractVersion ver1_1_1(1, 1, 1, ModularSmartcontractVersion::TYPE_TILDE);
	ModularSmartcontractVersion ver0_1_1(0, 1, 1, ModularSmartcontractVersion::TYPE_TILDE);
	ModularSmartcontractVersion ver0_0_1(0, 0, 1, ModularSmartcontractVersion::TYPE_TILDE);

	bool bl = ver1_1_1.validate(&ver1_1_1);
	CHECK(bl == true);
}
