/*
 * test_first_net.cpp
 *
 *  Created on: 2023/08/17
 *      Author: iizuka
 */
#include "test_utils/t_macros.h"

#include "bc/CodablecashConfig.h"
#include "bc/DebugDefaultLogger.h"

#include "../utils/DebugCodablecashConfigSetup.h"
#include "../wallet_util/WalletDriver.h"
#include "../../test_utils/TestPortSelection.h"

#include "bc_network_instance/ArrayNetworkSheeder.h"
#include "bc_network_instance/CodablecashNetworkNode.h"
#include "bc_network_instance/CodablecashNetworkNodeConfig.h"
#include "bc_network_instance/GenesisBalanceConfig.h"

#include "bc_base/AddressDescriptor.h"

#include "bc_block_generator/MiningConfig.h"

#include "bc_network/NodeIdentifierSource.h"
#include "bc_network/NodeIdentifier.h"

#include "bc_p2p_info/P2pNodeRecord.h"
using namespace codablecash;

TEST_GROUP(TestFirstNetGroup) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};

TEST(TestFirstNetGroup, case01){
	StackTestPortGetter portSel;
	int port01 = portSel.allocPort();
	int port02 = portSel.allocPort();

	File projectFolder = this->env->testCaseDir();

	File* dirWallet01 = projectFolder.get(L"wallet01"); __STP(dirWallet01);
	File* dirNode01 = projectFolder.get(L"node01"); __STP(dirNode01);
	File* dirNode02 = projectFolder.get(L"node02"); __STP(dirNode02);

	DebugDefaultLogger logger;

	CodablecashConfig config;
	DebugCodablecashConfigSetup::setupConfig02(config);

	CodablecashNetworkNodeConfig nwconfig;
	nwconfig.setSysConfig(&config);

	CodablecashNetworkNodeConfig* config01 = new CodablecashNetworkNodeConfig(nwconfig); __STP(config01);
	CodablecashNetworkNodeConfig* config02 = new CodablecashNetworkNodeConfig(nwconfig); __STP(config02);

	WalletDriver* walletDriver = new WalletDriver(0, dirWallet01, L"changeit"); __STP(walletDriver);
	walletDriver->init(1);
	{
		GenesisBalanceConfig gconfig;
		AddressDescriptor* desc = walletDriver->getAddressDesc(0, 0); __STP(desc);
		BalanceUnit amount(1000L * 10000L * 10000L);
		gconfig.addBalance(amount, desc);

		config01->setGenesisConfig(&gconfig);
		config01->setPort(port01);

		{
			MiningConfig minerConfig;
			AddressDescriptor* desc2 = walletDriver->getAddressDesc(0, 1); __STP(desc2);
			minerConfig.setAddressDescriptor(desc2);
			config01->setMinerConfig(&minerConfig);
		}

		NodeIdentifierSource* networkKey =  NodeIdentifierSource::create(); __STP(networkKey);
		config01->setNetworkKey(networkKey);
	}


	ArrayNetworkSheeder seeder;

	CodablecashNetworkNode node01(dirNode01, config01, &logger);
	{
		// First
		node01.generateNetwork(0); // zone 0

		// after init
		node01.startNetwork(&seeder, false);

		node01.startMiner();
		node01.startStakePool();

		{
			uint16_t zone = 0;
			const NodeIdentifierSource* source = config01->getNetworkKey();
			NodeIdentifier nodeId = source->toNodeIdentifier();
			const wchar_t* host = L"::1";
			int port = config01->getPort();

			P2pNodeRecord* rec = P2pNodeRecord::createIpV6Record(zone, &nodeId, host, port); __STP(rec);
			seeder.addRecord(rec);
		}
	}

	{
		config02->setPort(port02);
	}
	CodablecashNetworkNode node02(dirNode02, config02, &logger);
	{
		// second
		node02.initBlank(0); // zone 0

		// after init
		node02.startNetwork(&seeder, true);
		node02.syncNetwork();
	}


	// FIXME first test
}
