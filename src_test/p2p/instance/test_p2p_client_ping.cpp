/*
 * test_p2p_client_ping.cpp
 *
 *  Created on: 2023/09/09
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"

#include "bc_p2p_client/P2pClient.h"

#include "../../test_utils/TestPortSelection.h"
#include "bc/CodablecashConfig.h"

#include "bc/DebugDefaultLogger.h"

#include "bc/CodablecashNodeInstance.h"

#include "base/Exception.h"

#include "bc_p2p_client/P2pClientConnectionException.h"
using namespace codablecash;

TEST_GROUP(TestP2pClientPingGroup) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};

TEST(TestP2pClientPingGroup, case01){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"codable_home"))

	StackTestPortGetter portSel;
	int port = portSel.allocPort();

	CodablecashConfig config;
	config.setPowHashrateBlocks(10);
	config.setPowBlockTimeMills(500);


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

		P2pClient client(0, &logger);
		UnicodeString strLocal(L"::1");

		client.connectIpV6(&strLocal, port);

		client.close();
	}
}

TEST(TestP2pClientPingGroup, case02_err){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"codable_home"))

	StackTestPortGetter portSel;
	int port = portSel.allocPort();

	CodablecashConfig config;
	config.setPowHashrateBlocks(10);
	config.setPowBlockTimeMills(500);


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

		P2pClient client(0, &logger);
		UnicodeString strLocal(L"::1");
		client.connectIpV6(&strLocal, port);

		P2pClientConnectionException* ex = nullptr;
		try{
			client.__connectLogin(&strLocal, port);
		}
		catch(P2pClientConnectionException* e){
			ex = e;
		}
		CHECK(ex != nullptr);
		delete ex;
	}
}

TEST(TestP2pClientPingGroup, testEx01){
	testException<P2pClientConnectionException>();
}

TEST(TestP2pClientPingGroup, case03){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"codable_home"))

	StackTestPortGetter portSel;
	int port = portSel.allocPort();

	CodablecashConfig config;
	config.setPowHashrateBlocks(10);
	config.setPowBlockTimeMills(500);


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

		P2pClient client(0, &logger);
		UnicodeString strLocal(L"::1");

		client.connectIpV6(&strLocal, port);

		bool res = client.ping();
		CHECK(res == true);

		client.close();
	}
}

