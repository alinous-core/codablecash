/*
 * test_p2p_server.cpp
 *
 *  Created on: 2022/08/22
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"

#include "p2pserver/P2pServer.h"
#include "base/StackRelease.h"

#include "pubsub/PubSubId.h"
#include "pubsub/P2pHandshake.h"

#include "pubsub_cmd/EchoPubSubCommand.h"
#include "pubsub_cmd/AbstractCommandResponse.h"
#include "pubsub_cmd/EchoPubSubCommandResponse.h"
#include "pubsub_cmd/EndHandshakeCommand.h"

#include "bc_p2p_cmd/LoginPubSubCommand.h"

#include "bc/DebugDefaultLogger.h"

#include "p2pserver/P2pServerConnectionManager.h"

#include "../../test_utils/TestPortSelection.h"
#include "bc_p2p_cmd/LoginClientCommand.h"

#include "bc_p2p_cmd_client/DownloadDnsInfoCommand.h"

#include "bc_p2p_cmd_client/PingNodeCommand.h"
using namespace codablecash;

TEST_GROUP(P2PServerTestGroup) {
	TEST_SETUP() {}
	TEST_TEARDOWN() {}
};

TEST(P2PServerTestGroup, case01){
	DebugDefaultLogger logger;
	P2pServer* server = new P2pServer(&logger, nullptr); __STP(server);

	server->startIpV6Listening(nullptr, 54001);

	server->stopListning();

	server->close();
}

// -g P2PServerTestGroup -t case02
TEST(P2PServerTestGroup, case02){
	DebugDefaultLogger logger;
	P2pServer* server = new P2pServer(&logger, nullptr); __STP(server);

	StackTestPortGetter portSel;
	int port = portSel.allocPort();

	server->startIpV6Listening(nullptr, port);

	{
		PubSubId* psId = PubSubId::createNewId(); __STP(psId);
		P2pHandshake *handshake = new P2pHandshake(psId, &logger); __STP(handshake);

		UnicodeString strLocal(L"::1");
		handshake->connectIpV6(&strLocal, port);

		handshake->sendEndConnectionCommand();
		handshake->dispose();
	}

	server->stopListning();

	server->close();
}

TEST(P2PServerTestGroup, case03){
	DebugDefaultLogger logger;
	P2pServer* server = new P2pServer(&logger, nullptr); __STP(server);

	StackTestPortGetter portSel;
	int port = portSel.allocPort();

	server->startIpV6Listening(nullptr, port);

	const wchar_t* mysessage = L"Hello world";
	{
		PubSubId* psId = PubSubId::createNewId(); __STP(psId);
		P2pHandshake *handshake = new P2pHandshake(psId, &logger); __STP(handshake);

		UnicodeString strLocal(L"::1");
		handshake->connectIpV6(&strLocal, port);

		EchoPubSubCommand cmd;
		cmd.setMessage(mysessage);
		AbstractCommandResponse* res = handshake->publishCommand(&cmd); __STP(res);

		uint32_t type = res->getType();
		CHECK(AbstractCommandResponse::TYPE_RES_ECHO == type);

		EchoPubSubCommandResponse* echoResponse = dynamic_cast<EchoPubSubCommandResponse*>(res);
		const UnicodeString* message = echoResponse->getMessage();

		CHECK(message->equals(mysessage));

		handshake->sendEndConnectionCommand();
		handshake->dispose();

		P2pServerConnectionManager* conmgr = server->getConnectionManager();
		conmgr->removeDisabledHandshake();
	}

	server->stopListning();
	server->close();
}

TEST(P2PServerTestGroup, misc01){
	EchoPubSubCommand cmd;
	EchoPubSubCommand* cmd2 = dynamic_cast<EchoPubSubCommand*>(cmd.copyData()); __STP(cmd2);
	CHECK(cmd2 != nullptr);
}

TEST(P2PServerTestGroup, misc02){
	EndHandshakeCommand cmd;
	EndHandshakeCommand* cmd2 = dynamic_cast<EndHandshakeCommand*>(cmd.copyData()); __STP(cmd2);
	CHECK(cmd2 != nullptr);
}

TEST(P2PServerTestGroup, misc03){
	LoginPubSubCommand cmd(0);
	LoginPubSubCommand* cmd2 = dynamic_cast<LoginPubSubCommand*>(cmd.copyData()); __STP(cmd2);
	CHECK(cmd2 != nullptr);
}

TEST(P2PServerTestGroup, misc04){
	LoginClientCommand cmd(0);
	LoginClientCommand* cmd2 = dynamic_cast<LoginClientCommand*>(cmd.copyData()); __STP(cmd2);
	CHECK(cmd2 != nullptr);
}

TEST(P2PServerTestGroup, misc05){
	DownloadDnsInfoCommand cmd(0);
	DownloadDnsInfoCommand* cmd2 = dynamic_cast<DownloadDnsInfoCommand*>(cmd.copyData()); __STP(cmd2);
	CHECK(cmd2 != nullptr);
}

TEST(P2PServerTestGroup, misc06){
	PingNodeCommand cmd;
	PingNodeCommand* cmd2 = dynamic_cast<PingNodeCommand*>(cmd.copyData()); __STP(cmd2);
	CHECK(cmd2 != nullptr);
}
