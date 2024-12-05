/*
 * test_p2p_client_connection.cpp
 *
 *  Created on: 2023/08/11
 *      Author: iizuka
 */
#include "test_utils/t_macros.h"

#include "bc_network/NodeIdentifierSource.h"
#include "bc_network/NodeIdentifier.h"

#include "bc/DebugDefaultLogger.h"
#include "bc/CodablecashNodeInstance.h"
#include "bc/CodablecashSystemParam.h"

#include "pubsub/PubSubId.h"
#include "pubsub/P2pHandshake.h"

#include "pubsub_cmd/AbstractCommandResponse.h"

#include "bc_p2p_cmd/LoginClientCommand.h"

#include "../../test_utils/TestPortSelection.h"

using namespace codablecash;

TEST_GROUP(TestP2pClientConnectionGroup) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};


TEST(TestP2pClientConnectionGroup, case01){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"codable_home"))
	_ST(File, walletBaseDir, projectFolder.get(L"wallet"))

	NodeIdentifierSource* source = NodeIdentifierSource::create(); __STP(source);
	NodeIdentifier nodeId = source->toNodeIdentifier();

	CodablecashSystemParam config;
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
		inst.startNetwork(nullptr, port);

		{
			PubSubId* psId = PubSubId::createNewId(); __STP(psId);
			P2pHandshake *handshake = new P2pHandshake(psId, &logger); __STP(handshake);

			UnicodeString strLocal(L"::1");
			handshake->connectIpV6(&strLocal, port);

			{
				LoginClientCommand cmd(0);
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
	}
}

TEST(TestP2pClientConnectionGroup, case02_err){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"codable_home"))
	_ST(File, walletBaseDir, projectFolder.get(L"wallet"))

	NodeIdentifierSource* source = NodeIdentifierSource::create(); __STP(source);
	NodeIdentifier nodeId = source->toNodeIdentifier();
	NodeIdentifierSource* source2 = NodeIdentifierSource::create(); __STP(source2);
	NodeIdentifier nodeId2 = source2->toNodeIdentifier();

	CHECK(nodeId.compareTo(&nodeId2) != 0);

	CodablecashSystemParam config;
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
		inst.startNetwork(nullptr, port);
		{
			PubSubId* psId = PubSubId::createNewId(); __STP(psId);
			P2pHandshake *handshake = new P2pHandshake(psId, &logger); __STP(handshake);

			UnicodeString strLocal(L"::1");
			handshake->connectIpV6(&strLocal, port);

			handshake->getExecutor();

			{
				LoginClientCommand cmd(0);
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
