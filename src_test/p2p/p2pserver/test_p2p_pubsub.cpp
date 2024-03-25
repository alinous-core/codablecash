/*
 * test_p2p_pubsub.cpp
 *
 *  Created on: 2023/01/16
 *      Author: iizuka
 */



#include "test_utils/t_macros.h"

#include "base/StackRelease.h"

#include "dummy/DummyThread01.h"
#include "dummy/DummyThread02.h"
#include "dummy/DummyThread03.h"
#include "dummy/DummyServer.h"

#include "ipconnect/IpV6ClientConnection.h"

#include "pubsub/CommandPublisher.h"
#include "pubsub/PubSubId.h"
#include "pubsub/P2pHandshake.h"
#include "pubsub/CommandSubscriberThread.h"

#include "bc/DebugDefaultLogger.h"

#include "p2pserver/P2pServer.h"

#include "base_io/ByteBuffer.h"

#include "pubsub/HandshakeConnectionException.h"

#include "dummy/DebugP2pHandShake.h"
#include "pubsub/PubsubUtils.h"

#include "pubsub/PubsubCommandException.h"

#include "../../test_utils/TestPortSelection.h"
#include "bc_p2p_cmd/SignatureVerifivcationErrorException.h"

using namespace codablecash;

TEST_GROUP(P2pPubsubTestGroup) {
	TEST_SETUP() {}
	TEST_TEARDOWN() {}
};

TEST(P2pPubsubTestGroup, CommandPublisher01) {
	DebugDefaultLogger logger;

	StackTestPortGetter portSel;
	int port = portSel.allocPort();

	DummyServer* server = new DummyServer(); __STP(server);
	server->startIpV6Listening(nullptr, port, new DummyThread01());
	{
		IpV6ClientConnection* client = new IpV6ClientConnection(); __STP(client);
		client->connect(nullptr, port);

		PubSubId* psId = PubSubId::createNewId(); __STP(psId);
		CommandPublisher publisher(__STP_MV(client), psId);

		publisher.sendEndConnectionCommand(&logger);
	}
}

TEST(P2pPubsubTestGroup, CommandPublisher02) {
	DebugDefaultLogger logger;

	StackTestPortGetter portSel;
	int port = portSel.allocPort();

	DummyServer* server = new DummyServer(); __STP(server);
	server->startIpV6Listening(nullptr, port, new DummyThread02());
	{
		IpV6ClientConnection* client = new IpV6ClientConnection(); __STP(client);
		client->connect(nullptr, port);

		PubSubId* psId = PubSubId::createNewId(); __STP(psId);
		CommandPublisher publisher(__STP_MV(client), psId);

		publisher.sendEndConnectionCommand(&logger);
	}
}

TEST(P2pPubsubTestGroup, CommandSubscriberThread01) {
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

		CommandPublisher* publisher = handshake->getPublisher();

		IClientSocket* client = publisher->moveSocket(); __STP(client);
		uint8_t ch = 1;
		int n = client->write((const char*)&ch, sizeof(ch));
		CHECK(n == 1);
	}
	server->stopListning();
	server->close();
}

TEST(P2pPubsubTestGroup, CommandSubscriberThread02) {
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

		{
			handshake->addRef();
			handshake->decRef();
			int ref = handshake->getRef();
			CHECK(ref == 0);
		}

		CommandPublisher* publisher = handshake->getPublisher();

		IClientSocket* client = publisher->moveSocket(); __STP(client);
		ByteBuffer* buff = ByteBuffer::allocateWithEndian(10, true); __STP(buff);
		buff->putInt(CommandSubscriberThread::MAX_COMMAND_SIZE + 1);

		buff->position(0);
		const char* ar = (const char*)buff->array();

		int n = client->write(ar, buff->limit());
		CHECK(n == buff->limit());
	}
	server->stopListning();
	server->close();
}


TEST(P2pPubsubTestGroup, P2pHandshake01) {
	DebugDefaultLogger logger;
	PubSubId* psId = PubSubId::createNewId(); __STP(psId);

	StackTestPortGetter portSel;
	int port = portSel.allocPort();

	DummyServer* server = new DummyServer(); __STP(server);
	server->startIpV6Listening(nullptr, port, new DummyThread03());
	{
		DebugP2pHandShake* handshake = new DebugP2pHandShake(psId, &logger); __STP(handshake);

		IpV6ClientConnection* client = new IpV6ClientConnection(54001); __STP(client);
		client->connect(nullptr, port);

		HandshakeConnectionException* ex = nullptr;
		try{
			handshake->__test_connectPublisher(__STP_MV(client));
		}catch(Exception* e){
			ex = dynamic_cast<HandshakeConnectionException*>(e);
		}
		CHECK(ex != nullptr);
		delete ex;
	}
}

TEST(P2pPubsubTestGroup, P2pHandshake02) {
	DebugDefaultLogger logger;
	PubSubId* psId = PubSubId::createNewId(); __STP(psId);

	StackTestPortGetter portSel;
	int port = portSel.allocPort();

	DummyServer* server = new DummyServer(); __STP(server);
	server->startIpV6Listening(nullptr, port, new DummyThread03());
	{
		DebugP2pHandShake* handshake = new DebugP2pHandShake(psId, &logger); __STP(handshake);

		IpV6ClientConnection* client = new IpV6ClientConnection(); __STP(client);
		client->connect(nullptr, port);

		HandshakeConnectionException* ex = nullptr;
		try{
			handshake->__test_connectSubscriber(__STP_MV(client));
		}catch(Exception* e){
			ex = dynamic_cast<HandshakeConnectionException*>(e);
		}
		CHECK(ex != nullptr);
		delete ex;
	}
}

TEST(P2pPubsubTestGroup, P2pHandshake03) {
	DebugDefaultLogger logger;
	PubSubId* psId = PubSubId::createNewId(); __STP(psId);

	{
		P2pHandshake* handshake = new P2pHandshake(psId, &logger); __STP(handshake);

		HandshakeConnectionException* ex = nullptr;
		try{
			handshake->publishCommand(nullptr);
		}catch(Exception* e){
			ex = dynamic_cast<HandshakeConnectionException*>(e);
		}
		CHECK(ex != nullptr);
		delete ex;
	}
}

TEST(P2pPubsubTestGroup, P2pHandshake04) {
	DebugDefaultLogger logger;
	PubSubId* psId = PubSubId::createNewId(); __STP(psId);

	{
		IpV6ClientConnection* client = new IpV6ClientConnection(); __STP(client);

		DebugP2pHandShake* handshake = new DebugP2pHandShake(psId, &logger); __STP(handshake);
		HandshakeConnectionException* ex = nullptr;
		try{
			handshake->__test_checkConnection(client);
		}catch(Exception* e){
			ex = dynamic_cast<HandshakeConnectionException*>(e);
		}
		CHECK(ex != nullptr);
		delete ex;
	}
}

TEST(P2pPubsubTestGroup, PubsubUtils01) {
	IpV6ClientConnection* client = new IpV6ClientConnection(); __STP(client);

	PubsubCommandException* ex = nullptr;
	try{
		PubsubUtils::readAll(client, 100);
	}catch(Exception* e){
		ex = dynamic_cast<PubsubCommandException*>(e);
	}
	CHECK(ex != nullptr);
	delete ex;
}

TEST(P2pPubsubTestGroup, ConnectException01) {
	IpV6ClientConnection con;

	PubsubCommandException* ex = nullptr;
	try{
		CommandSubscriberThread::checkSocketConnected(&con);
	}
	catch(PubsubCommandException* e){
		ex = e;
	}

	CHECK(ex != nullptr);
	delete ex;
}

TEST(P2pPubsubTestGroup, SignatureVerifivcationErrorException01) {
	testException<SignatureVerifivcationErrorException>();
}

