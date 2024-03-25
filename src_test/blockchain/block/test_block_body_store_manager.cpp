/*
 * test_block_store.cpp
 *
 *  Created on: 2023/01/25
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"

#include "bc_blockstore_body/BlockBodyStoreManager.h"

#include "../wallet_util/WalletDriver.h"

#include "bc_block_body/BlockBody.h"

using namespace codablecash;

TEST_GROUP(TestBlockBodyStoreGroup) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};

TEST(TestBlockBodyStoreGroup, case01){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"store"))
	_ST(File, walletBaseDir, projectFolder.get(L"wallet"))

	WalletDriver driver(0, walletBaseDir, L"changeit");
	driver.init(2);
	driver.initGenesis(0, 100000);

	BlockBodyStoreManager* store = new BlockBodyStoreManager(baseDir, 2); __STP(store);
	{
		BalanceTransferTransaction* trx = driver.createBalanceTransferTransaction(0, 1, 0, BalanceUnit(10000), BalanceUnit(1), false);
		__STP(trx);

		BlockBody body;
		body.addBalanceTransaction(trx);
		body.build();
		const BlockMerkleRoot* root = body.getMerkleRoot();

		store->addBlockBody(&body, 1);

		BlockBody* body2 = store->getBlockBody(root, 1); __STP(body2);

		store->removeBlockBody(root, 1);

		BlockBody* body3 = store->getBlockBody(root, 1); __STP(body3);
		CHECK(body3 == nullptr);
	}
}

TEST(TestBlockBodyStoreGroup, case02){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"store"))
	_ST(File, walletBaseDir, projectFolder.get(L"wallet"))

	WalletDriver driver(0, walletBaseDir, L"changeit");
	driver.init(2);
	driver.initGenesis(0, 100000);

	BlockBodyStoreManager* store = new BlockBodyStoreManager(baseDir, 2); __STP(store);
	{
		BalanceTransferTransaction* trx = driver.createBalanceTransferTransaction(0, 1, 0, BalanceUnit(10000), BalanceUnit(1), false);
		__STP(trx);

		BlockBody body;
		body.addBalanceTransaction(trx);
		body.build();
		const BlockMerkleRoot* root = body.getMerkleRoot();

		store->addBlockBody(&body, 1);

		store->addBlockBody(&body, 10);

		store->addBlockBody(&body, 20);

		BlockBody* body2 = store->getBlockBody(root, 1); __STP(body2);

		store->removeBlockBody(root, 1);

		BlockBody* body3 = store->getBlockBody(root, 1); __STP(body3);
		CHECK(body3 == nullptr);

		BlockBody* body4 = store->getBlockBody(root, 30); __STP(body4);
		CHECK(body4 == nullptr);
	}
}

