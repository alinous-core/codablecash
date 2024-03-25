/*
 * test_block_genesis.cpp
 *
 *  Created on: 2023/01/25
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"

#include "bc_block/BlockHeader.h"
#include "bc_block/Block.h"

#include "bc_blockstore/CodablecashBlockchain.h"
#include "bc_blockstore/BlockchainStoreException.h"

#include "blockchain/wallet_util/WalletDriver.h"

#include "bc_trx_genesis/GenesisTransaction.h"


using namespace codablecash;

TEST_GROUP(TestBlockGenesisGroup) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};

TEST(TestBlockGenesisGroup, genesis03){
	File projectFolder = this->env->testCaseDir();
	_ST(File, walletBaseDir, projectFolder.get(L"wallet"))

	WalletDriver driver(0, walletBaseDir, L"changeit");
	driver.init(2);
	driver.initGenesis(0, 100000);

	GenesisTransaction* genTrx = driver.createGenesisTransaction(0, 10000); __STP(genTrx);

	BalanceUnit feeRate = genTrx->getFeeRate();
	CHECK(feeRate.getAmount() == 0);

	BalanceUnit fee = genTrx->getFee();
	CHECK(fee.getAmount() == 0);

	CHECK(genTrx->getUtxoReference(0) == nullptr);

	CHECK(genTrx->validateFinal(nullptr, nullptr, nullptr) == TrxValidationResult::OK);
	CHECK(genTrx->validateOnAccept(nullptr, nullptr));
}

TEST(TestBlockGenesisGroup, genesis02){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"codable_home"))
	_ST(File, walletBaseDir, projectFolder.get(L"wallet"))

	WalletDriver driver(0, walletBaseDir, L"changeit");
	driver.init(2);
	driver.initGenesis(0, 100000);

	{
		uint16_t zoneSelf = 0;
		CodablecashBlockchain::createBlankBlockchain(baseDir, zoneSelf, 1);
	}

	{
		CodablecashBlockchain chain(baseDir);
		chain.open();

		Block block(0, 1);
		CHECK(block.getHeight() == 1);

		block.setGenesisTimestamp();

		GenesisTransaction* genTrx = driver.createGenesisTransaction(0, 10000); __STP(genTrx);
		block.addBalanceTransaction(genTrx);
		block.build();

		chain.addBlock(nullptr, &block);

		chain.close();
	}
}


TEST(TestBlockGenesisGroup, genesis01){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"codable_home"))

	{
		uint16_t zoneSelf = 0;
		CodablecashBlockchain::createBlankBlockchain(baseDir, zoneSelf, 1);
	}

	{
		BlockchainStoreException* ex = nullptr;
		try{
			// error case
			uint16_t zoneSelf = 0;
			CodablecashBlockchain::createBlankBlockchain(baseDir, zoneSelf, 1);
		}
		catch(BlockchainStoreException* e){
			ex = e;
		}
		CHECK(ex != nullptr);
		delete ex;
	}

	{
		CodablecashBlockchain chain(baseDir);
	}
}

TEST(TestBlockGenesisGroup, checkEx01){
	testException<BlockchainStoreException>();
}


