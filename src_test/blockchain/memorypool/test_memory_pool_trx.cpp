/*
 * test_memory_pool_trx.cpp
 *
 *  Created on: 2023/02/28
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"

#include "blockchain/wallet_util/WalletDriver.h"

#include "bc_memorypool/MemoryPool.h"
#include "bc_memorypool/MemPoolTransaction.h"
#include "bc_memorypool/FeeBananceTransactionScanner.h"

using namespace codablecash;

TEST_GROUP(TestMemoryPoolTrxGroup) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};

TEST(TestMemoryPoolTrxGroup, case01){
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
		MemoryPool pool(baseDir);
		pool.open();
		pool.putTransaction(trx);
		pool.putTransaction(trx2);
		pool.close();
	}

	{
		MemoryPool pool(baseDir);
		pool.open();
		{
			MemPoolTransaction* tx = pool.begin(); __STP(tx);
			tx->consume(trxId);
			{
				FeeBananceTransactionScanner* scanner = tx->beginScanBalanceTransaction(); __STP(scanner);
				while(scanner->hasNext()){
					AbstractBalanceTransaction* t = scanner->next(); __STP(t);
					const TransactionId* id = t->getTransactionId();
					CHECK(id->equals(trxId2));
				}
			}
		}
		pool.close();
	}
}

TEST(TestMemoryPoolTrxGroup, case02){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"pool"))
	_ST(File, walletBaseDir, projectFolder.get(L"wallet"))

	WalletDriver driver(0, walletBaseDir, L"changeit");
	driver.init(2);
	driver.initGenesis(0, 100000);

	BalanceTransferTransaction* trx = driver.createBalanceTransferTransaction(0, 1, 0, BalanceUnit(10000), BalanceUnit(1), false);
	__STP(trx);
	BalanceTransferTransaction* trx2 = driver.createBalanceTransferTransaction(0, 1, 0, BalanceUnit(5000), BalanceUnit(2), false);
	__STP(trx2);
	const TransactionId* trxId = trx->getTransactionId();
	const TransactionId* trxId2 = trx2->getTransactionId();
	{
		MemoryPool pool(baseDir);
		pool.open();
		pool.putTransaction(trx);
		pool.putTransaction(trx2);
		pool.close();
	}

	{
		MemoryPool pool(baseDir);
		pool.open();
		{
			MemPoolTransaction* tx = pool.begin(); __STP(tx);
			tx->consume(trxId);
			{
				FeeBananceTransactionScanner* scanner = tx->beginScanBalanceTransaction(); __STP(scanner);
				while(scanner->hasNext()){
					AbstractBalanceTransaction* t = scanner->next(); __STP(t);
					const TransactionId* id = t->getTransactionId();
					CHECK(id->equals(trxId2));
				}
			}
		}
		pool.close();
	}
}
