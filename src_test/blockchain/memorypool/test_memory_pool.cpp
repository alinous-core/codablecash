/*
 * test_memory_pool.cpp
 *
 *  Created on: 2023/02/27
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"

#include "blockchain/wallet_util/WalletDriver.h"

#include "bc_memorypool/MemoryPool.h"
#include "bc_memorypool/MemoryPoolException.h"

#include "bc_trx_genesis/GenesisTransaction.h"

#include "bc_trx_balance/BalanceTransferTransaction.h"
#include "bc_trx_balance/BalanceUtxo.h"

#include "bc_base_trx_index/BalanceUnitKey.h"
#include "bc_base_trx_index/TransactionIdData.h"
#include "bc_base_trx_index/TransactionIdsListData.h"

#include "bc_trx/TransactionId.h"
#include "bc_trx/UtxoId.h"

#include "btreekey/InfinityKey.h"
#include "btreekey/NullKey.h"

using namespace codablecash;

TEST_GROUP(TestMemoryPoolGroup) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};

TEST(TestMemoryPoolGroup, case01){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"pool"))

	MemoryPool pool(baseDir);
	pool.createBlankPool();

	pool.open();
	pool.close();

	bool bl = pool.exists();
	CHECK(bl);

	pool.createBlankPool();
}

TEST(TestMemoryPoolGroup, case02){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"pool"))

	MemoryPool pool(baseDir);

	pool.open();
	pool.close();

	bool bl = pool.exists();
	CHECK(bl);
}

TEST(TestMemoryPoolGroup, case03_err){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"pool"))
	_ST(File, walletBaseDir, projectFolder.get(L"wallet"))

	WalletDriver driver(0, walletBaseDir, L"changeit");
	driver.init(2);

	MemoryPool pool(baseDir);
	pool.open();

	MemoryPoolException* ex = nullptr;
	try{
		GenesisTransaction* trx = nullptr; //driver.createGenesisTransaction(0, 100000); __STP(trx);
		pool.putTransaction(trx);
	}
	catch(MemoryPoolException* e){
		ex = e;
	}
	CHECK(ex != nullptr);
	delete ex;

	pool.close();

	testException<MemoryPoolException>();
}

TEST(TestMemoryPoolGroup, case04){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"pool"))
	_ST(File, walletBaseDir, projectFolder.get(L"wallet"))

	WalletDriver driver(0, walletBaseDir, L"changeit");
	driver.init(2);
	driver.initGenesis(0, 100000);

	BalanceTransferTransaction* trx = driver.createBalanceTransferTransaction(0, 1, 0, BalanceUnit(10000), BalanceUnit(1), false);
	__STP(trx);
	const TransactionId* trxId = trx->getTransactionId();
	{
		MemoryPool pool(baseDir);
		pool.open();
		pool.putTransaction(trx);
		pool.close();
	}

	{
		MemoryPool pool(baseDir);
		pool.open();

		AbstractBalanceTransaction* trx2 = pool.getBalanceTransaction(trxId); __STP(trx2);
		CHECK(trxId->equals(trx2->getTransactionId()));

		int maxLoop = trx->getUtxoSize();
		for(int i = 0; i != maxLoop; ++i){
			const BalanceUtxo* utxo = dynamic_cast<BalanceUtxo*>(trx->getUtxo(i));
			const UtxoId* utxoId = utxo->getId();

			const TransactionId* id = pool.getTransactionIdFromUtxoId(utxoId); __STP(id);
			CHECK(trxId->equals(id));
		}

		pool.close();
	}
}

TEST(TestMemoryPoolGroup, removeTrx01){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"pool"))
	_ST(File, walletBaseDir, projectFolder.get(L"wallet"))

	WalletDriver driver(0, walletBaseDir, L"changeit");
	driver.init(2);
	driver.initGenesis(0, 100000);

	BalanceTransferTransaction* trx = driver.createBalanceTransferTransaction(0, 1, 0, BalanceUnit(10000), BalanceUnit(1), false);
	__STP(trx);
	const TransactionId* trxId = trx->getTransactionId();
	{
		MemoryPool pool(baseDir);
		pool.open();
		pool.putTransaction(trx);
		pool.close();
	}

	{
		MemoryPool pool(baseDir);
		pool.open();

		pool.removeTransaction(trxId);

		AbstractBalanceTransaction* trx2 = pool.getBalanceTransaction(trxId); __STP(trx2);
		CHECK(trx2 == nullptr);

		pool.close();
	}
}

TEST(TestMemoryPoolGroup, removeTrx02){
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

		pool.removeTransaction(trxId);

		AbstractBalanceTransaction* dtrx = pool.getBalanceTransaction(trxId); __STP(dtrx);
		CHECK(dtrx == nullptr);

		pool.close();
	}

	{
		MemoryPool pool(baseDir);
		pool.open();

		pool.removeTransaction(trxId2);

		AbstractBalanceTransaction* dtrx = pool.getBalanceTransaction(trxId); __STP(dtrx);
		CHECK(dtrx == nullptr);

		pool.close();
	}

	{
		TransactionIdData data(trxId);
		TransactionIdData* data2 = dynamic_cast<TransactionIdData*>(data.copyData()); __STP(data2);

		CHECK(trxId->equals(data2->getTrxId()));
	}
}

TEST(TestMemoryPoolGroup, misc01){
	BalanceUnit bu(10L);
	BalanceUnitKey key(&bu);
	InfinityKey sinf;
	NullKey nullkey;

	CHECK(key.compareTo(&sinf) < 0);
	CHECK(key.compareTo(&nullkey) > 0);
}

TEST(TestMemoryPoolGroup, misc02){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"pool"))
	_ST(File, walletBaseDir, projectFolder.get(L"wallet"))

	WalletDriver driver(0, walletBaseDir, L"changeit");
	driver.init(2);
	driver.initGenesis(0, 100000);

	BalanceTransferTransaction* trx = driver.createBalanceTransferTransaction(0, 1, 0, BalanceUnit(10000), BalanceUnit(1), false);
	__STP(trx);
	const TransactionId* trxId = trx->getTransactionId();

	TransactionIdsListData data;

	int index = data.indexof(trxId);
	CHECK(index < 0);

	data.add(trxId);
	TransactionIdsListData* data2 = dynamic_cast<TransactionIdsListData*>(data.copyData()); __STP(data2);
	data.join(data2);
}
