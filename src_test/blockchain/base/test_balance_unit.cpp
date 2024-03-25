/*
 * test_balance_unit.cpp
 *
 *  Created on: 2023/02/20
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"

#include "bc_base/BalanceUnit.h"

using namespace codablecash;

TEST_GROUP(TestBalanceUnitGroup) {
	TEST_SETUP(){}
	TEST_TEARDOWN(){}
};

TEST(TestBalanceUnitGroup, case01){
	BalanceUnit unit(100);
	BalanceUnit* unit2 = dynamic_cast<BalanceUnit*>(unit.copyData()); __STP(unit2);

	CHECK(unit2->getAmount() == 100);
}

TEST(TestBalanceUnitGroup, case02){
	BalanceUnit unit(100);
	unit = unit - BalanceUnit(10);

	CHECK(unit.getAmount() == 90);
}

