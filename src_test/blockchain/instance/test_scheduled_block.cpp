/*
 * test_scheduled_block.cpp
 *
 *  Created on: Sep 7, 2025
 *      Author: iizuka
 */
#include "test_utils/t_macros.h"

#include "bc/CodablecashSystemParam.h"
#include "bc/DebugDefaultLogger.h"
#include "bc/CodablecashNodeInstance.h"

#include "../wallet_util/WalletDriver.h"

#include "bc_block/Block.h"
#include "bc_block/BlockHeader.h"

#include "bc_status_cache/BlockchainController.h"

#include "bc_block_generator/MiningConfig.h"

#include "base_timestamp/SystemTimestamp.h"

#include "bc_trx_genesis/GenesisTransaction.h"

#include "pow/PoWNonce.h"

#include "bc_status_cache_context/IStatusCacheContext.h"

#include "bc_memorypool/MemoryPool.h"

#include "bc_memorypool/MemPoolTransaction.h"
using namespace codablecash;


TEST_GROUP(TestScheduledBlockGroup) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};


TEST(TestScheduledBlockGroup, case01){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"codable_home"))
	_ST(File, walletBaseDir, projectFolder.get(L"wallet"))

	CodablecashSystemParam config;
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

		inst.suspendBlockGenerator();

		{
			BlockHeader* topHeader = ctrl->getTopHeader(); __STP(topHeader);

			Block block(0, topHeader->getHeight() + 1);
			const SystemTimestamp* tm = block.getTimestamp();
			SystemTimestamp diff(1, 0);
			diff = diff + *tm;
			block.setNonceGeneratedTimestamp(&diff);
			block.setLastHeaderId(topHeader->getId());

			block.setPoWNonce(&PoWNonce::MAX_NONCE);

			block.build();

			ctrl->setScheduledBlock(&block);

			MemoryPool* memPool = inst.getMemoryPool();
			MemPoolTransaction* memTrx = memPool->begin(); __STP(memTrx);
			ctrl->requestMiningBlock(memTrx);

			IStatusCacheContext* context = ctrl->getStatusCacheContext(0); __STP(context);
		}

		inst.shutdown();
	}
}
