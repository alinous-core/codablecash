/*
 * test_dummy_utxo.cpp
 *
 *  Created on: 2023/05/13
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"

#include "bc_base/BalanceUnit.h"

#include "bc_wallet/HdWalletSeed.h"

#include "bc_status_cache_context/RemovedDummyUtxo.h"

using namespace codablecash;

TEST_GROUP(TestDummyUtxoGroup) {
	TEST_SETUP(){}
	TEST_TEARDOWN(){}
};

TEST(TestDummyUtxoGroup, case01){
	RemovedDummyUtxo utxo;

	BalanceUnit unit = utxo.getAmount();

	CHECK(utxo.getAddress() == nullptr);
	utxo.build();
}

