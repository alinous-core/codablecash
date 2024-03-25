/*
 * test_transaction_id.cpp
 *
 *  Created on: 2023/02/20
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"

#include "bc_base/BalanceUnit.h"

#include "bc_wallet/HdWalletSeed.h"

using namespace codablecash;

TEST_GROUP(Test32byteidGroup) {
	TEST_SETUP(){}
	TEST_TEARDOWN(){}
};

TEST(Test32byteidGroup, case01){
	HdWalletSeed* seed = HdWalletSeed::newSeed(); __STP(seed);

	int size = seed->size();
	CHECK(size == 32);

	seed->toArray();
}
