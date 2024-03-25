/*
 * test_omitted_body.cpp
 *
 *  Created on: 2023/12/04
 *      Author: iizuka
 */
#include "test_utils/t_macros.h"

#include <cstdint>

#include "../utils/InstanceDriver.h"
#include "../../test_utils/TestPortSelection.h"
#include "../utils/NodeConnectionSimulator.h"
#include "../utils/DebugCodablecashConfigSetup.h"
#include "../utils/ValidationInstance.h"

#include "bc/CodablecashConfig.h"
#include "bc/DebugDefaultLogger.h"

#include "bc_block_generator/MiningConfig.h"

#include "bc_base/BalanceUnit.h"

#include "bc_trx_balance/BalanceTransferTransaction.h"

#include "bc_status_cache/BlockchainController.h"

#include "bc_trx/NopInterChainCommunicationTransaction.h"

#include "bc_smartcontract/NopSmartcontractTransaction.h"

#include "bc_p2p_cmd_node/DownloadOmittedBlockBodyNodeCommand.h"

#include "bc_block/Block.h"
#include "bc_block/BlockHeader.h"

#include "pubsub_cmd/AbstractCommandResponse.h"

#include "bc_p2p_cmd_node/DownloadOmittedBlockBodyNodeCommandResponse.h"

#include "bc_block_body/OmittedBlockBody.h"

#include "bc_block_body/BlockBody.h"

#include "bc_block/BlockMerkleRoot.h"
using namespace codablecash;

TEST_GROUP(TestOmittedBodyGroup) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};

TEST(TestOmittedBodyGroup, case01){
	StackTestPortGetter portSel;
	int port01 = portSel.allocPort();

	File projectFolder = this->env->testCaseDir();
	InstanceDriver driver(&projectFolder);

	CodablecashConfig config;
	DebugCodablecashConfigSetup::setupConfig01(config);
	config.setPowBlockTimeMills(10);

	driver.initWallet(1);
	driver.initInstance(&config);

	driver.startV6Listner(port01);

	MiningConfig mconfig;
	driver.startMiner(&mconfig);
	driver.startFinalize();

	driver.suspendMiner();

	driver.initGenesisBalance(0, 10000L * 1000L * 1000L);
	driver.registerVotingNode();


	int maxLoop = 30;
	for(int i = 0; i != maxLoop; ++i) {
		BalanceUnit stakeAmount(1000L);
		driver.registerTicket(stakeAmount);
	}

	{
		BalanceUnit feeRate(2L);
		BalanceUnit amount(100L);
		driver.sendBalance(amount, feeRate);
	}
	{
		BalanceUnit feeRate(3L);
		BalanceUnit amount(200L);
		driver.sendBalance(amount, feeRate);
	}
	{
		NopInterChainCommunicationTransaction nopTrx;
		nopTrx.build();

		driver.sentTransaction(&nopTrx);
	}
	{
		NopSmartcontractTransaction nopTrx;
		nopTrx.build();

		driver.sentTransaction(&nopTrx);
	}

	{
		BlockchainController* ctrl = driver.getBlockchainController();
		int count = ctrl->getMempoolTrxCount();
		CHECK(count == 35)
	}
	driver.resumeMiner();

	uint64_t wheight = 5;
	driver.waitUntilFinalizedHeight(wheight);
	driver.suspendMiner();

	// ommit body
	{
		DebugDefaultLogger* logger = driver.getLogger();
		ValidationInstance validator(&projectFolder);
		validator.initInstance(&config, logger);

		File* simDir = projectFolder.get(L"sim"); __STP(simDir);
		UnicodeString host(L"::1");
		NodeConnectionSimulator sim(driver.getLogger(), simDir);
		sim.connectIpv6Node(0, &host, port01);

		BlockchainController* ctrl = driver.getBlockchainController();
		uint64_t maxH = wheight + 1;
		for(uint64_t h = 1; h != maxH; h++){
			ArrayList<Block>* list = ctrl->getBlocksHeightAt(0, h); __STP(list);
			list->setDeleteOnExit();

			int maxLoop = list->size();
			for(int i = 0; i != maxLoop; ++i){
				const Block* block = list->get(i);
				const BlockHeader* header = block->getHeader();
				uint16_t zone = header->getZone();
				uint64_t height = header->getHeight();
				const BlockHeaderId* headerId = header->getId();


				DownloadOmittedBlockBodyNodeCommand cmd;
				cmd.setZone(zone);
				cmd.setHeight(height);
				cmd.setHeaderId(headerId);

				{
					AbstractCommandResponse* r = sim.signAndAbstractNodeCommand(&cmd); __STP(r);
					DownloadOmittedBlockBodyNodeCommandResponse* response = dynamic_cast<DownloadOmittedBlockBodyNodeCommandResponse*>(r);

					const OmittedBlockBody* obody = response->getBody();

					BlockBody* body2 = obody->toBlockBody(height, headerId, &sim); __STP(body2);

					{
						const BlockMerkleRoot* root2 = body2->getMerkleRoot();
						const BlockMerkleRoot* root = header->getMerkleRoot();

						bool bl = root2->equals(root);
						CHECK(bl);
					}

					Block* block2 = new Block(dynamic_cast<BlockHeader*>(header->copyData()), __STP_MV(body2)); __STP(block2);
					block2->build();

					validator.validateBlock(block2);
				}

			}

			// misc
		}

		sim.close();
	}

}

