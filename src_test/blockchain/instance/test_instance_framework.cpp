/*
 * test_instance_framework.cpp
 *
 *  Created on: 2023/03/17
 *      Author: iizuka
 */
#include "test_utils/t_macros.h"

#include "bc/CodablecashNodeInstance.h"
#include "bc/DebugDefaultLogger.h"

#include "../wallet_util/WalletDriver.h"

#include "bc_trx_genesis/GenesisTransaction.h"

#include "bc_status_cache/BlockchainController.h"
#include "bc_status_cache/BlockIdChangedException.h"

#include "bc_block/Block.h"

#include "bc/CodablecashConfig.h"

#include "bc_block_generator/MiningConfig.h"

#include "base_timestamp/SystemTimestamp.h"

using namespace codablecash;

TEST_GROUP(TestInstanceFrameworkGroup) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};


/**
 * BlockchainStatusCache#report2PowManager()
 * PoWStartCalculateCommandMessage#process() command
 * After calculated block
 * BlockGenerator::nonceCalculated() called.
 *
 *
 */
TEST(TestInstanceFrameworkGroup, case02){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"codable_home"))
	_ST(File, walletBaseDir, projectFolder.get(L"wallet"))

	CodablecashConfig config;
	config.setPowHashrateBlocks(10);
	config.setPowBlockTimeMills(500);

	DebugDefaultLogger logger;
	{
		CodablecashNodeInstance inst(baseDir, &logger, &config);
		bool bl = inst.initBlankInstance(0, 1);
		CHECK(bl == true);
	}

	{
		CodablecashNodeInstance inst(baseDir, &logger, &config);
		inst.load();

		// miner
		MiningConfig mconfig;
		inst.startBlockGenerator(&mconfig);

		BlockchainController* ctrl = inst.getController();
		{
			WalletDriver driver(0, walletBaseDir, L"changeit");
			driver.init(1);

			GenesisTransaction* gtrx = driver.createGenesisTransaction(0, 1000000); __STP(gtrx);
			Block block(0, 1);
			block.setGenesisTimestamp();

			block.addBalanceTransaction(gtrx);

			const SystemTimestamp* tm = block.getTimestamp();
			SystemTimestamp diff(1, 0);
			diff = diff + *tm;
			block.setNonceGeneratedTimestamp(&diff);

			block.build();

			ctrl->addBlock(&block);
		}

		uint64_t height;
		do{
			Os::usleep(120000);
			height = ctrl->getHeadHeight(0);
		}
		while(height < 2);

		inst.shutdown();
	}
}


TEST(TestInstanceFrameworkGroup, case01){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"codable_home"))

	CodablecashConfig config;
	config.setPowHashrateBlocks(10);
	config.setPowBlockTimeMills(500);

	DebugDefaultLogger logger;
	{
		CodablecashNodeInstance inst(baseDir, &logger, &config);
		bool bl = inst.initBlankInstance(0, 1);
		CHECK(bl == true);

		// This test throws Exception
		bl = inst.initBlankInstance(0, 1);
		CHECK(bl == false);
	}

	{
		CodablecashNodeInstance inst(baseDir, &logger, &config);
		inst.load();

		inst.shutdown();
	}
}

TEST(TestInstanceFrameworkGroup, BlockIdChangedException01){
	testException<BlockIdChangedException>();
}
