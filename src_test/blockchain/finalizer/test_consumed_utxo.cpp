/*
 * test_consumed_utxo.cpp
 *
 *  Created on: 2023/05/27
 *      Author: iizuka
 */
#include "test_utils/t_macros.h"

#include "../utils/InstanceDriver.h"
#include "bc_block_generator/MiningConfig.h"

#include "bc/CodablecashNodeInstance.h"
#include "bc/CodablecashSystemParam.h"

#include "bc_status_cache/BlockchainController.h"

#include "bc_status_cache_context/IStatusCacheContext.h"

#include "bc_wallet/WalletAccount.h"

#include "../wallet_util/WalletDriver.h"
#include "bc_trx/AbstractControlTransaction.h"

#include "bc_block/Block.h"

#include "bc_trx/AbstractUtxoReference.h"

#include "bc_finalizer_trx/RegisterVotePoolTransaction.h"

#include "../utils/DebugCodablecashSystemParamSetup.h"
#include "dummy/FinalizeMissAndBlankblockTestAllocator.h"

using namespace codablecash;

TEST_GROUP(TestConsumedUtxoGroup) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};

TEST(TestConsumedUtxoGroup, case01){
	File projectFolder = this->env->testCaseDir();
	InstanceDriver driver(&projectFolder);

	CodablecashSystemParam param;
	DebugCodablecashSystemParamSetup::setupConfig01(param);
	param.setPowBlockTimeMills(10);

	driver.initWallet(1);

	driver.initInstance(&param);

	MiningConfig mconfig;
	driver.startMiner(&mconfig);
	driver.startFinalize();

	driver.suspendMiner();

	driver.initGenesisBalance(0, 10000L * 1000L * 1000L);
	driver.registerVotingNode();

/*	driver.resumeMiner();

	driver.waitUntilHeight(2);*/

	{
		BlockchainController* ctrl = driver.getBlockchainController();
		WalletDriver* wallet = driver.getWallet();
		WalletAccount* account = wallet->getAccount(0);

		ArrayList<AbstractBlockchainTransaction>* list = account->getTransactions(); __STP(list);
		list->setDeleteOnExit();

		AbstractBlockchainTransaction* t0 = list->get(0);
		AbstractBlockchainTransaction* t1 = list->get(1);

		AbstractControlTransaction* trx = dynamic_cast<AbstractControlTransaction*>(t1);
		AbstractUtxoReference* ref = t1->getUtxoReference(0);
		if(ref == nullptr){
			trx = dynamic_cast<AbstractControlTransaction*>(t0);
			ref = t0->getUtxoReference(0);
		}

		{
			IStatusCacheContext* context = ctrl->getStatusCacheContext(0); __STP(context);

			Block block(0, 2);
			BlockHeader* header = block.getHeader();

			context->importControlTransaction(header, trx);

			const UtxoId* utxoId = ref->getUtxoId();
			AbstractUtxo* utxo = context->getUtxo(utxoId);
			CHECK(utxo == nullptr);
		}
		{
			IStatusCacheContext* context = ctrl->getStatusCacheContext(0); __STP(context);

			Block block(0, 2);
			BlockHeader* header = block.getHeader();

			context->importControlTransaction(header, trx);

			const UtxoId* utxoId = ref->getUtxoId();
			AbstractUtxo* utxo = context->getUtxo(utxoId);
			CHECK(utxo == nullptr);
		}

		{
			IStatusCacheContext* context = ctrl->getStatusCacheContext(0); __STP(context);

			Block block(0, 2);
			BlockHeader* header = block.getHeader();

			context->importControlTransaction(header, trx);

			//
			RegisterVotePoolTransaction* regTrx = dynamic_cast<RegisterVotePoolTransaction*>(trx);
			const NodeIdentifier* nodeId = regTrx->getNodeId();

			const VoterEntry* entry = context->getVoterEntry(nodeId);
			CHECK(entry != nullptr)
		}

		{
			IStatusCacheContext* context = ctrl->getStatusCacheContext(0); __STP(context);

			//
			RegisterVotePoolTransaction* regTrx = dynamic_cast<RegisterVotePoolTransaction*>(trx);
			const NodeIdentifier* nodeId = regTrx->getNodeId();

			const VoterEntry* entry = context->getVoterEntry(nodeId);
			CHECK(entry == nullptr)
		}

	}
}

