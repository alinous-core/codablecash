/*
 * test_wallnet_mempool.cpp
 *
 *  Created on: Jul 26, 2025
 *      Author: iizuka
 */
#include "test_utils/t_macros.h"

#include "bc_wallet_net_data/WalletMemoryPool.h"

#include "../wallet_util/WalletDriver.h"

using namespace codablecash;


TEST_GROUP(TestWalletMempool) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};

TEST(TestWalletMempool, case01){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"pool"))

	{
		WalletMemoryPool pool(baseDir);

		pool.open();
		pool.close();
	}

	{
		WalletMemoryPool pool(baseDir);

		pool.createBlankPool();

		pool.open();
		pool.close();
	}
}


TEST(TestWalletMempool, case02){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"pool"))
	_ST(File, walletBaseDir, projectFolder.get(L"wallet"))

	WalletDriver driver(0, walletBaseDir, L"changeit");
	driver.init(2);
	driver.initGenesis(0, 100000);

	BalanceTransferTransaction* trx = driver.createBalanceTransferTransaction(0, 1, 0, BalanceUnit(10000), BalanceUnit(1), false);
	__STP(trx);
	BalanceTransferTransaction* trx2 = driver.createBalanceTransferTransaction(0, 1, 0, BalanceUnit(10000), BalanceUnit(1), false);
	__STP(trx2);
	const TransactionId* trxId = trx->getTransactionId();
	const TransactionId* trxId2 = trx2->getTransactionId();


	{
		WalletMemoryPool pool(baseDir);
		pool.open();

		pool.putTransaction(trx);
		pool.putTransaction(trx2);

		{
			AbstractBlockchainTransaction* t =pool.getBlockchainTransaction(trxId); __STP(t);
			CHECK(t != nullptr)
		}
		{
			AbstractBlockchainTransaction* t =pool.getBlockchainTransaction(trxId2); __STP(t);
			CHECK(t != nullptr)
		}

		pool.removeTransaction(trxId);
		{
			AbstractBlockchainTransaction* t =pool.getBlockchainTransaction(trxId); __STP(t);
			CHECK(t == nullptr)
		}


		pool.close();
	}
}
