/*
 * test_p2p_server_connection.cpp
 *
 *  Created on: 2023/01/16
 *      Author: iizuka
 */


#include "test_utils/t_macros.h"

#include "base_io/ByteBuffer.h"

#include "base/UnicodeString.h"
#include "base/StackRelease.h"
#include "base/Exception.h"

#include "p2pserver/AbstractConnectionCommand.h"
#include "p2pserver/P2pServer.h"
#include "p2pserver/ConnectPublisherCommand.h"
#include "p2pserver/ConnectSubscriberCommand.h"
#include "p2pserver/P2pServerConnectionManager.h"

#include "bc/DebugDefaultLogger.h"

#include "pubsub/PubSubId.h"
#include "pubsub/P2pHandshake.h"

#include "dummy/DummyServer.h"
#include "dummy/DummyThread01.h"
#include "dummy/DummyThread02.h"

#include "ipconnect/IpV6ClientConnection.h"

#include "../../test_utils/TestPortSelection.h"
#include "p2pserver/WrongPubsubConnectionCommandException.h"
using namespace codablecash;

TEST_GROUP(P2PServerConnectionTestGroup) {
	TEST_SETUP() {}
	TEST_TEARDOWN() {}
};

TEST(P2PServerConnectionTestGroup, AbstractConnectionCommand01){
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(100, true); __STP(buff);
	buff->putInt(0);
	buff->position(0);

	WrongPubsubConnectionCommandException* ex = nullptr;
	try{
		AbstractConnectionCommand* cmd = AbstractConnectionCommand::createFromBinary(buff);
	}
	catch(WrongPubsubConnectionCommandException* e){
		ex = e;
	}

	CHECK(ex != nullptr);
	delete ex;
}

TEST(P2PServerConnectionTestGroup, WrongPubsubConnectionCommandException01){
	testException<WrongPubsubConnectionCommandException>();
}


TEST(P2PServerConnectionTestGroup, AbstractConnectionCommand02){
	DummyServer* server = new DummyServer(); __STP(server);

	StackTestPortGetter portSel;
	int port = portSel.allocPort();

	server->startIpV6Listening(nullptr, port, new DummyThread01());
	{
		PubSubId* psId = PubSubId::createNewId(); __STP(psId);
		ConnectPublisherCommand cmd(psId);

		IpV6ClientConnection* client = new IpV6ClientConnection(); __STP(client);
		client->connect(nullptr, port);

		cmd.sendCommand(client);

		Exception* ex = nullptr;
		try{
			cmd.reveiveResponse(client);
		}
		catch(Exception* e){
			ex = e;
		}
		CHECK(ex != nullptr);
		delete ex;
	}
}

TEST(P2PServerConnectionTestGroup, AbstractConnectionCommand03){
	DummyServer* server = new DummyServer(); __STP(server);

	StackTestPortGetter portSel;
	int port = portSel.allocPort();

	server->startIpV6Listening(nullptr, port, new DummyThread02());
	{
		PubSubId* psId = PubSubId::createNewId(); __STP(psId);
		ConnectPublisherCommand cmd(psId);

		IpV6ClientConnection* client = new IpV6ClientConnection(); __STP(client);
		client->connect(nullptr, port);

		cmd.sendCommand(client);

		Exception* ex = nullptr;
		try{
			cmd.reveiveResponse(client);
		}
		catch(Exception* e){
			ex = e;
		}
		CHECK(ex != nullptr);
		delete ex;
	}
}

TEST(P2PServerConnectionTestGroup, ConnectPublisherCommand01){
	DebugDefaultLogger logger;
	P2pServer* server = new P2pServer(&logger, nullptr); __STP(server);

	StackTestPortGetter portSel;
	int port = portSel.allocPort();

	server->startIpV6Listening(nullptr, port);

	{
		UnicodeString strLocal(L"::1");
		IpV6ClientConnection* client = new IpV6ClientConnection(); __STP(client);
		client->connect(&strLocal, port);

		PubSubId* psId = PubSubId::createNewId(); __STP(psId);
		ConnectPublisherCommand cmd(psId);

		cmd.sendCommand(client);
		cmd.reveiveResponse(client);

		CHECK(!cmd.isOk());
	}

	server->stopListning();
	server->close();
}

TEST(P2PServerConnectionTestGroup, ConnectSubscriberCommand01){
	DebugDefaultLogger logger;
	P2pServer* server = new P2pServer(&logger, nullptr); __STP(server);

	StackTestPortGetter portSel;
	int port = portSel.allocPort();

	server->startIpV6Listening(nullptr, port);
	{
		UnicodeString strLocal(L"::1");

		PubSubId* psId = PubSubId::createNewId(); __STP(psId);
		{
			IpV6ClientConnection* client = new IpV6ClientConnection(); __STP(client);
			client->connect(&strLocal, port);
			ConnectSubscriberCommand cmd(psId);

			cmd.sendCommand(client);
			cmd.reveiveResponse(client);
		}

		{
			IpV6ClientConnection* client = new IpV6ClientConnection(); __STP(client);
			client->connect(&strLocal, port);
			ConnectSubscriberCommand cmd(psId);

			cmd.sendCommand(client);
			cmd.reveiveResponse(client);

			CHECK(!cmd.isOk());
		}
	}
	server->stopListning();
	server->close();
}

TEST(P2PServerConnectionTestGroup, ConnectSubscriberCommand02) {
	DebugDefaultLogger logger;
	P2pServer* server = new P2pServer(&logger, nullptr); __STP(server);

	StackTestPortGetter portSel;
	int port = portSel.allocPort();

	server->startIpV6Listening(nullptr, port);
	{
		UnicodeString strLocal(L"::1");
		PubSubId* psId = PubSubId::createNewId(); __STP(psId);

		P2pHandshake *handshake = new P2pHandshake(psId, &logger); __STP(handshake);
		handshake->connectIpV6(&strLocal, port);

		{
			IpV6ClientConnection* client = new IpV6ClientConnection(); __STP(client);
			client->connect(&strLocal, port);
			ConnectSubscriberCommand cmd(psId);

			cmd.sendCommand(client);
			cmd.reveiveResponse(client);

			CHECK(!cmd.isOk());
		}

		handshake->dispose();
	}
	server->stopListning();
	server->close();
}

TEST(P2PServerConnectionTestGroup, P2pConnectionAcceptThread01) {
	DebugDefaultLogger logger;
	P2pServer* server = new P2pServer(&logger, nullptr); __STP(server);

	StackTestPortGetter portSel;
	int port = portSel.allocPort();

	server->startIpV6Listening(nullptr, port);
	{
		UnicodeString strLocal(L"::1");
		IpV6ClientConnection* client = new IpV6ClientConnection(); __STP(client);
		client->connect(&strLocal, port);

		uint8_t ch = 1;
		int n = client->write((const char*)&ch, sizeof(ch));
		CHECK(n == 1);
	}
	server->stopListning();
	server->close();
}

TEST(P2PServerConnectionTestGroup, P2pConnectionAcceptThread02) {
	DebugDefaultLogger logger;
	P2pServer* server = new P2pServer(&logger, nullptr); __STP(server);

	StackTestPortGetter portSel;
	int port = portSel.allocPort();

	server->startIpV6Listening(nullptr, port);
	{
		UnicodeString strLocal(L"::1");
		IpV6ClientConnection* client = new IpV6ClientConnection(); __STP(client);
		client->connect(&strLocal, port);

		uint32_t ch = 200000;
		int n = client->write((const char*)&ch, sizeof(ch));
		CHECK(n == 4);
	}
	server->stopListning();
	server->close();
}

TEST(P2PServerConnectionTestGroup, P2pServerConnectionManager01){
	DebugDefaultLogger logger;
	P2pServer* server = new P2pServer(&logger, nullptr); __STP(server);

	StackTestPortGetter portSel;
	int port = portSel.allocPort();

	P2pServerConnectionManager* conManager = server->getConnectionManager();
	conManager->setExpireSec(0);

	server->startIpV6Listening(nullptr, port);
	{
		UnicodeString strLocal(L"::1");

		IpV6ClientConnection* client = new IpV6ClientConnection(); __STP(client);
		client->connect(&strLocal, port);

		PubSubId* psId = PubSubId::createNewId(); __STP(psId);
		ConnectSubscriberCommand cmd(psId);
		cmd.sendCommand(client);
		cmd.reveiveResponse(client);

		CHECK(cmd.isOk());

		Os::usleep(1500);

		conManager->__testRemoveExpiredWaitingPublisher();
	}
	server->stopListning();
	server->close();
}

