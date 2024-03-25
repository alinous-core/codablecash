/*
 * test_p2p_instance.cpp
 *
 *  Created on: 2023/04/05
 *      Author: iizuka
 */
#include "test_utils/t_macros.h"

#include "bc/CodablecashNodeInstance.h"
#include "bc/DebugDefaultLogger.h"
#include "bc/CodablecashConfig.h"

#include "bc_trx_genesis/GenesisTransaction.h"

#include "bc_status_cache/BlockchainController.h"
#include "bc_status_cache/BlockIdChangedException.h"

#include "bc_block/Block.h"

#include "bc_block_generator/MiningConfig.h"

#include "pubsub/PubSubId.h"
#include "pubsub/P2pHandshake.h"

#include "pubsub_cmd/AbstractCommandResponse.h"

#include "bc_network/NodeIdentifierSource.h"
#include "bc_network/NodeIdentifier.h"

#include "bc_p2p_cmd/LoginPubSubCommand.h"
#include "bc_p2p_cmd/P2pHandshakeAuthenticationException.h"

#include "bc_p2p/BlockchainZoneException.h"

#include "pubsub/PubsubCommandException.h"

#include "../../test_utils/TestPortSelection.h"
using namespace codablecash;

TEST_GROUP(TestP2pInstanceGroup) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};

TEST(TestP2pInstanceGroup, case01){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"codable_home"))
	_ST(File, walletBaseDir, projectFolder.get(L"wallet"))

	NodeIdentifierSource* source = NodeIdentifierSource::create(); __STP(source);
	NodeIdentifier nodeId = source->toNodeIdentifier();

	CodablecashConfig config;
	config.setPowHashrateBlocks(10);
	config.setPowBlockTimeMills(500);

	StackTestPortGetter portSel;
	int port = portSel.allocPort();

	DebugDefaultLogger logger;
	{
		CodablecashNodeInstance inst(baseDir, &logger, &config);
		bool bl = inst.initBlankInstance(0, 1);
		CHECK(bl == true);
	}

	{
		CodablecashNodeInstance inst(baseDir, &logger, &config);
		inst.load();
		inst.startNetwork(port);
		{
			PubSubId* psId = PubSubId::createNewId(); __STP(psId);
			P2pHandshake *handshake = new P2pHandshake(psId, &logger); __STP(handshake);

			UnicodeString strLocal(L"::1");
			handshake->connectIpV6(&strLocal, port);

			{
				LoginPubSubCommand cmd(0);
				cmd.sign(source);
				bool res = cmd.verify();
				CHECK(res == true);

				AbstractCommandResponse* response = handshake->publishCommand(&cmd); __STP(response);
				uint32_t type = response->getType();
				CHECK(type == AbstractCommandResponse::TYPE_RES_OK);
			}

			handshake->sendEndConnectionCommand();
			handshake->dispose();
		}

		inst.shutdownNetwork();
		inst.shutdown();
	}
}

TEST(TestP2pInstanceGroup, case02_err){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"codable_home"))
	_ST(File, walletBaseDir, projectFolder.get(L"wallet"))

	NodeIdentifierSource* source = NodeIdentifierSource::create(); __STP(source);
	NodeIdentifier nodeId = source->toNodeIdentifier();
	NodeIdentifierSource* source2 = NodeIdentifierSource::create(); __STP(source2);
	NodeIdentifier nodeId2 = source2->toNodeIdentifier();

	CHECK(nodeId.compareTo(&nodeId2) != 0);

	CodablecashConfig config;
	config.setPowHashrateBlocks(10);
	config.setPowBlockTimeMills(500);

	StackTestPortGetter portSel;
	int port = portSel.allocPort();

	DebugDefaultLogger logger;
	{
		CodablecashNodeInstance inst(baseDir, &logger, &config);
		bool bl = inst.initBlankInstance(0, 1);
		CHECK(bl == true);
	}

	{
		CodablecashNodeInstance inst(baseDir, &logger, &config);
		inst.load();
		inst.startNetwork(port);
		{
			PubSubId* psId = PubSubId::createNewId(); __STP(psId);
			P2pHandshake *handshake = new P2pHandshake(psId, &logger); __STP(handshake);

			UnicodeString strLocal(L"::1");
			handshake->connectIpV6(&strLocal, port);

			handshake->getExecutor();

			{
				LoginPubSubCommand cmd(0);
				cmd.sign(source);
				cmd.setNodeIdentifier(&nodeId2);

				bool res = cmd.verify();
				CHECK(res == false);

				AbstractCommandResponse* response = handshake->publishCommand(&cmd); __STP(response);
				uint32_t type = response->getType();
				CHECK(type == AbstractCommandResponse::TYPE_RES_SIGNATURE_ERROR);
			}

			handshake->sendEndConnectionCommand();
			handshake->dispose();
		}

		inst.shutdownNetwork();
		inst.shutdown();
	}
}

/**
 *
 */
TEST(TestP2pInstanceGroup, case03_err){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"codable_home"))
	_ST(File, walletBaseDir, projectFolder.get(L"wallet"))

	NodeIdentifierSource* source = NodeIdentifierSource::create(); __STP(source);
	NodeIdentifier nodeId = source->toNodeIdentifier();

	CodablecashConfig config;
	config.setPowHashrateBlocks(10);
	config.setPowBlockTimeMills(500);

	int port = 54001;

	DebugDefaultLogger logger;
	{
		CodablecashNodeInstance inst(baseDir, &logger, &config);
		bool bl = inst.initBlankInstance(0, 1);
		CHECK(bl == true);
	}

	{
		CodablecashNodeInstance inst(baseDir, &logger, &config);
		inst.load();
		inst.startNetwork(port);
		{
			PubSubId* psId = PubSubId::createNewId(); __STP(psId);
			P2pHandshake *handshake = new P2pHandshake(psId, &logger); __STP(handshake);

			UnicodeString strLocal(L"::1");
			handshake->connectIpV6(&strLocal, 54001);

			{
				LoginPubSubCommand cmd(0);
				cmd.sign(source);
				bool res = cmd.verify();
				CHECK(res == true);

				AbstractCommandResponse* response = handshake->publishCommand(&cmd); __STP(response);
				uint32_t type = response->getType();
				CHECK(type == AbstractCommandResponse::TYPE_RES_OK);
			}

			{
				LoginPubSubCommand cmd(0);
				cmd.sign(source);
				bool res = cmd.verify();
				CHECK(res == true);

				PubsubCommandException* ex = nullptr;
				try{
					AbstractCommandResponse* response = handshake->publishCommand(&cmd); __STP(response);
					if(response->getType() != AbstractCommandResponse::TYPE_RES_OK){
						throw new PubsubCommandException(__FILE__, __LINE__);
					}
				}
				catch(PubsubCommandException* e){
					ex = e;
				}
				CHECK(ex != nullptr);
				delete ex;
			}

			handshake->dispose();
		}

		inst.shutdownNetwork();
		inst.shutdown();
	}
}

TEST(TestP2pInstanceGroup, P2pHandshakeAuthenticationException01){
	testException<P2pHandshakeAuthenticationException>();
}

TEST(TestP2pInstanceGroup, P2pHandshakeAuthenticationException02){
	testException<BlockchainZoneException>();
}

