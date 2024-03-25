/*
 * test_nop_transactions.cpp
 *
 *  Created on: 2023/12/04
 *      Author: iizuka
 */
#include "test_utils/t_macros.h"

#include "bc_trx/NopInterChainCommunicationTransaction.h"

#include "bc_smartcontract/NopSmartcontractTransaction.h"

#include "bc_base/BalanceUnit.h"
using namespace codablecash;
//using namespace alinous;

TEST_GROUP(TestNopTransactionsGroup) {
	TEST_SETUP(){}
	TEST_TEARDOWN(){}
};

TEST(TestNopTransactionsGroup, case01){
	NopInterChainCommunicationTransaction trx;

	CHECK(trx.getUtxoSize() == 0);
	CHECK(trx.getUtxo(0) == nullptr);
	CHECK(trx.getUtxoReferenceSize() == 0);
	CHECK(trx.getUtxoReference(0) == nullptr);

	BalanceUnit fee = trx.getFee();
	BalanceUnit zero(0L);
	CHECK(fee.compareTo(&zero) == 0);

	BalanceUnit rate = trx.getFeeRate();
	CHECK(rate.compareTo(&zero) == 0);
}

TEST(TestNopTransactionsGroup, case02){
	NopSmartcontractTransaction trx;

	CHECK(trx.getUtxoSize() == 0);
	CHECK(trx.getUtxo(0) == nullptr);
	CHECK(trx.getUtxoReferenceSize() == 0);
	CHECK(trx.getUtxoReference(0) == nullptr);

	BalanceUnit fee = trx.getFee();
	BalanceUnit zero(0L);
	CHECK(fee.compareTo(&zero) == 0);

	BalanceUnit rate = trx.getFeeRate();
	CHECK(rate.compareTo(&zero) == 0);
}

