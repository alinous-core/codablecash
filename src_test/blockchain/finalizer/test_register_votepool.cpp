/*
 * test_register_votepool.cpp
 *
 *  Created on: 2023/04/15
 *      Author: iizuka
 */
#include "test_utils/t_macros.h"

#include "bc/CodablecashConfig.h"
#include "bc/DebugDefaultLogger.h"
#include "bc/CodablecashNodeInstance.h"

#include "../wallet_util/WalletDriver.h"

#include "bc_block/Block.h"

#include "bc_block_generator/MiningConfig.h"

#include "bc_trx_genesis/GenesisTransaction.h"

#include "bc_status_cache/BlockchainController.h"

#include "bc_finalizer_trx/RegisterVotePoolTransaction.h"

#include "bc_network/NodeIdentifierSource.h"
#include "bc_network/NodeIdentifier.h"

#include "base/Exception.h"

#include "bc_memorypool/MemoryPool.h"

#include "bc_status_cache_context/StatusCacheContext.h"

#include "bc_finalizer/VoterEntry.h"

using namespace codablecash;

TEST_GROUP(TestRegisterVotePoolGroup) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};

TEST(TestRegisterVotePoolGroup, case01){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"codable_home"))
	_ST(File, walletBaseDir, projectFolder.get(L"wallet"))

	CodablecashConfig config;
	config.setPowHashrateBlocks(10);
	config.setPowBlockTimeMills(250);

	WalletDriver driver(0, walletBaseDir, L"changeit");
	driver.init(1);

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
			GenesisTransaction* gtrx = driver.createGenesisTransaction(0, 1000000); __STP(gtrx);
			Block block(0, 1);
			block.setGenesisTimestamp();
			block.addBalanceTransaction(gtrx);
			block.build();

			ctrl->addBlock(&block);
			driver.importTransaction(gtrx);
		}

		// register pool
		NodeIdentifierSource* source = NodeIdentifierSource::create(); __STP(source);
		NodeIdentifier nodeId = source->toNodeIdentifier();
		{
			RegisterVotePoolTransaction* trx = driver.createRegisterVotePoolTransaction(0, source, BalanceUnit(1L)); __STP(trx);
			bool res = trx->verify();
			CHECK(res == true);

			driver.importTransaction(trx);

			MemoryPool* pool = inst.getMemoryPool();
			pool->putTransaction(trx);
		}

		uint64_t height;
		do{
			Os::usleep(120000);
			height = ctrl->getHeadHeight(0);
		}
		while(height < 3);

		{
			IStatusCacheContext* context = ctrl->getStatusCacheContext(0); __STP(context);
			ArrayList<VoterEntry, VoterEntry::VoteCompare>* list = context->getVoterEntries(); __STP(list);

			int size = list->size();
			CHECK(size == 1);

			VoterEntry* entry = list->get(0);
			int cmp = entry->getNodeIdentifier()->compareTo(&nodeId);
			CHECK(cmp == 0);

			list->deleteElements();
		}

		inst.shutdown();
	}

}

TEST(TestRegisterVotePoolGroup, caseEror01){
	File projectFolder = this->env->testCaseDir();
	_ST(File, walletBaseDir, projectFolder.get(L"wallet"))

	WalletDriver driver(0, walletBaseDir, L"changeit");
	driver.init(1);

	NodeIdentifierSource* source = NodeIdentifierSource::create(); __STP(source);
	NodeIdentifier nodeId = source->toNodeIdentifier();

	Exception* ex = nullptr;
	try{
		RegisterVotePoolTransaction* trx = driver.createRegisterVotePoolTransaction(0, source, BalanceUnit(1L)); __STP(trx);
	}
	catch(Exception* e){
		ex = e;
	}
	CHECK(ex != nullptr);
	delete ex;

}
