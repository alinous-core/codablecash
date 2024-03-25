/*
 * test_control_trx.cpp
 *
 *  Created on: 2023/05/27
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"

#include "../utils/DebugCodablecashConfigSetup.h"
#include "../utils/InstanceDriver.h"

#include "../wallet_util/WalletDriver.h"

#include "base/StackRelease.h"
#include "base/Exception.h"

#include "bc_network/NodeIdentifierSource.h"
#include "bc_network/NodeIdentifier.h"

#include "bc_base/BalanceUnit.h"

#include "bc/CodablecashConfig.h"

#include "bc_block_generator/MiningConfig.h"

#include "bc_status_cache/BlockchainController.h"
#include "bc_status_cache_context/IStatusCacheContext.h"

#include "bc_block/Block.h"

#include "bc/CodablecashNodeInstance.h"

#include "bc_memorypool/MemoryPool.h"
#include "bc_memorypool/MemPoolTransaction.h"

#include "bc_finalizer_trx/RegisterTicketTransaction.h"
#include "bc_finalizer_trx/RegisterVotePoolTransaction.h"

using namespace codablecash;

TEST_GROUP(TestControlTrxGroup) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};

TEST(TestControlTrxGroup, regosterTicketError01){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"wallet"))

	WalletDriver driver(0, baseDir, L"changeit");
	driver.init(2);
	driver.initGenesis(0, 10);

	NodeIdentifierSource* source = NodeIdentifierSource::create(); __STP(source);
	NodeIdentifier nodeId = source->toNodeIdentifier();

	BalanceUnit stakeAmount(100L);
	BalanceUnit feeRate(100L);

	Exception* ex = nullptr;
	try{
		driver.createRegisterTicketTransaction(0, 0, 0, &nodeId, stakeAmount, feeRate);
	}
	catch(Exception* e){
		ex = e;
	}

	CHECK(ex != nullptr);
	delete ex;
}

TEST(TestControlTrxGroup, RegisterTicketTransaction01){
	File projectFolder = this->env->testCaseDir();
	InstanceDriver driver(&projectFolder);

	CodablecashConfig config;
	DebugCodablecashConfigSetup::setupConfig01(config);
	config.setPowBlockTimeMills(10);

	driver.initWallet(1);

	driver.initInstance(&config);

	MiningConfig mconfig;
	driver.startMiner(&mconfig);
	driver.startFinalize();

	driver.suspendMiner();

	driver.initGenesisBalance(0, 10000L * 1000L * 1000L);
	driver.registerVotingNode();

	driver.resumeMiner();

	driver.waitUntilHeight(3);

	{
		BalanceUnit feeRate(1L);
		BalanceUnit amount(200L);
		driver.sendBalance(amount, feeRate);
	}

	BlockchainController* ctrl = driver.getBlockchainController();
	MemoryPool* pool = driver.getInstance()->getMemoryPool();
	{
		// on memory
		{
			MemPoolTransaction* memTrx = pool->begin(); __STP(memTrx);
			IStatusCacheContext* context = ctrl->getStatusCacheContext(0); __STP(context);

			Block block(0, 2);
			BlockHeader* header = block.getHeader();

			BalanceUnit stakeAmount(1000L);
			RegisterTicketTransaction* trx = driver.createRegisterTicketTransaction(stakeAmount); __STP(trx);

			// test
			bool bl = trx->validateOnAccept(memTrx, context);
			CHECK(bl == true);

			TrxValidationResult res = trx->validateFinal(header, memTrx, context);
			CHECK(res == TrxValidationResult::PENDING);
		}

		// fee invalid
		{
			MemPoolTransaction* memTrx = pool->begin(); __STP(memTrx);
			IStatusCacheContext* context = ctrl->getStatusCacheContext(0); __STP(context);

			Block block(0, 2);
			BlockHeader* header = block.getHeader();

			BalanceUnit stakeAmount(1000L);
			RegisterTicketTransaction* trx = driver.createRegisterTicketTransaction(stakeAmount); __STP(trx);

			BalanceUnit fee(100L);
			trx->setFeeAmount(&fee);

			// test
			bool bl = trx->validateOnAccept(memTrx, context);
			CHECK(bl == false);

			TrxValidationResult res = trx->validateFinal(header, memTrx, context);
			CHECK(res == TrxValidationResult::INVALID);
		}

		// price
		{
			MemPoolTransaction* memTrx = pool->begin(); __STP(memTrx);
			IStatusCacheContext* context = ctrl->getStatusCacheContext(0); __STP(context);

			Block block(0, 2);
			BlockHeader* header = block.getHeader();

			BalanceUnit stakeAmount(1L);
			RegisterTicketTransaction* trx = driver.createRegisterTicketTransaction(stakeAmount); __STP(trx);

			// test
			TrxValidationResult res = trx->validateFinal(header, memTrx, context);
			CHECK(res == TrxValidationResult::INVALID);
		}

		// node identifier
		{
			MemPoolTransaction* memTrx = pool->begin(); __STP(memTrx);
			IStatusCacheContext* context = ctrl->getStatusCacheContext(0); __STP(context);

			Block block(0, 2);
			BlockHeader* header = block.getHeader();

			BalanceUnit stakeAmount(1000L);
			RegisterTicketTransaction* trx = driver.createRegisterTicketTransaction(stakeAmount); __STP(trx);

			NodeIdentifierSource* source = NodeIdentifierSource::create(); __STP(source);
			NodeIdentifier nodeId = source->toNodeIdentifier();
			trx->setNodeId(&nodeId);

			// test
			bool bl = trx->validateOnAccept(memTrx, context);
			CHECK(bl == false);

			TrxValidationResult res = trx->validateFinal(header, memTrx, context);
			CHECK(res == TrxValidationResult::INVALID);
		}
	}
}

TEST(TestControlTrxGroup, RegisterVotePoolTransaction01){
	File projectFolder = this->env->testCaseDir();
	InstanceDriver driver(&projectFolder);

	CodablecashConfig config;
	DebugCodablecashConfigSetup::setupConfig01(config);
	config.setPowBlockTimeMills(10);

	driver.initWallet(1);

	driver.initInstance(&config);

	MiningConfig mconfig;
	driver.startMiner(&mconfig);
	driver.startFinalize();

	driver.suspendMiner();

	driver.initGenesisBalance(0, 10000L * 1000L * 1000L);

	{
		BalanceUnit feeRate(1L);
		BalanceUnit amount(2000L);
		driver.sendBalance(amount, feeRate);
	}

	BlockchainController* ctrl = driver.getBlockchainController();
	MemoryPool* pool = driver.getInstance()->getMemoryPool();

	// on memory pool
	{
		MemPoolTransaction* memTrx = pool->begin(); __STP(memTrx);
		IStatusCacheContext* context = ctrl->getStatusCacheContext(0); __STP(context);

		Block block(0, 2);
		BlockHeader* header = block.getHeader();

		RegisterVotePoolTransaction* trx = driver.createRegisterVotePoolTransaction(); __STP(trx);

		// test
		bool bl = trx->validateOnAccept(memTrx, context);
		CHECK(bl == true);

		TrxValidationResult res = trx->validateFinal(header, memTrx, context);
		CHECK(res == TrxValidationResult::PENDING);
	}

	// wrong fee
	{
		MemPoolTransaction* memTrx = pool->begin(); __STP(memTrx);
		IStatusCacheContext* context = ctrl->getStatusCacheContext(0); __STP(context);

		Block block(0, 2);
		BlockHeader* header = block.getHeader();

		RegisterVotePoolTransaction* trx = driver.createRegisterVotePoolTransaction(); __STP(trx);

		BalanceUnit fee(100L);
		trx->setFeeAmount(&fee);

		// test
		bool bl = trx->validateOnAccept(memTrx, context);
		CHECK(bl == false);

		TrxValidationResult res = trx->validateFinal(header, memTrx, context);
		CHECK(res == TrxValidationResult::INVALID);
	}
}

TEST(TestControlTrxGroup, RegisterVotePoolTransaction02){
	File projectFolder = this->env->testCaseDir();
	InstanceDriver driver(&projectFolder);

	CodablecashConfig config;
	DebugCodablecashConfigSetup::setupConfig01(config);
	config.setPowBlockTimeMills(10);

	driver.initWallet(1);

	driver.initInstance(&config);

	MiningConfig mconfig;
	driver.startMiner(&mconfig);
	driver.startFinalize();

	driver.suspendMiner();

	driver.initGenesisBalance(0, 10000L * 1000L * 1000L);
	driver.registerVotingNode();

	driver.resumeMiner();

	driver.waitUntilHeight(3);

	BlockchainController* ctrl = driver.getBlockchainController();
	MemoryPool* pool = driver.getInstance()->getMemoryPool();

	// duplicated id
	{
		MemPoolTransaction* memTrx = pool->begin(); __STP(memTrx);
		IStatusCacheContext* context = ctrl->getStatusCacheContext(0); __STP(context);

		Block block(0, 2);
		BlockHeader* header = block.getHeader();

		RegisterVotePoolTransaction* trx = driver.createRegisterVotePoolTransaction(); __STP(trx);

		// test
		bool bl = trx->validateOnAccept(memTrx, context);
		CHECK(bl == false);

		TrxValidationResult res = trx->validateFinal(header, memTrx, context);
		CHECK(res == TrxValidationResult::INVALID);
	}

}


