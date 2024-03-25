/*
 * test_p2p_v4server_connection.cpp
 *
 *  Created on: 2023/09/09
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
#include "ipconnect/IpV4ClientConnection.h"
#include "ipconnect/UnexpectedProtocolException.h"
#include "ipconnect/IpV6ServerConnection.h"

#include "../../test_utils/TestPortSelection.h"

#include "bc_p2p/BlockchainZoneException.h"
#include "bc_p2p/BlockchainNodeHandshakeException.h"

#include "bc_network_instance_sync/MemPoolSyncWorkerException.h"

#include "bc_p2p_cmd_node_access/RetryableNode2NodeAccessException.h"

using namespace codablecash;

TEST_GROUP(P2PV4ServerConnectionTestGroup) {
	TEST_SETUP() {}
	TEST_TEARDOWN() {}
};

TEST(P2PV4ServerConnectionTestGroup, UnexpectedProtocolException01){
	testException<UnexpectedProtocolException>();

	UnexpectedProtocolException* ex = nullptr;
	try{
		IpV6ServerConnection::checkInetFamily(100);
	}
	catch(UnexpectedProtocolException* e){
		ex = e;
	}
	CHECK(ex != nullptr);
	delete ex;
}

TEST(P2PV4ServerConnectionTestGroup, AbstractConnectionCommand02){
	DummyServer* server = new DummyServer(); __STP(server);

	StackTestPortGetter portSel;
	int port = portSel.allocPort();

	server->startIpV6Listening(nullptr, port, new DummyThread01());
	{
		PubSubId* psId = PubSubId::createNewId(); __STP(psId);
		ConnectPublisherCommand cmd(psId);

		IpV4ClientConnection* client = new IpV4ClientConnection(); __STP(client);
		UnicodeString host(L"127.0.0.1");

		client->connect(&host, port);

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

TEST(P2PV4ServerConnectionTestGroup, testEx01){
	testException<BlockchainZoneException>();
	testException<BlockchainNodeHandshakeException>();
	testException<MemPoolSyncWorkerException>();
	testException<RetryableNode2NodeAccessException>();
}

