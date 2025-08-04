/*
 * TestnetSetupper01.cpp
 *
 *  Created on: Mar 5, 2025
 *      Author: iizuka
 */

#include "TestnetSetupper01.h"

#include "bc_network/NodeIdentifierSource.h"

#include "bc_network_instance/GenesisBalanceConfig.h"
#include "bc_network_instance/CodablecashNetworkNodeConfig.h"
#include "bc_network_instance/FinalizerConfig.h"

#include "bc_block_generator/MiningConfig.h"

#include "bc_base/BalanceUnit.h"
#include "bc_base/AddressDescriptor.h"

#include "../../utils_testnet/MultizoneTestnet.h"
#include "../../utils_testnet/TestnetInstanceWrapper.h"
#include "../../utils/DebugCodablecashSystemParamSetup.h"

#include "bc_wallet_net/NetworkWallet.h"

#include "base/StackRelease.h"

#include "bc/CodablecashSystemParam.h"


namespace codablecash {

MultizoneTestnet* TestnetSetupper01::setup2NodeZone0(NetworkWallet *wallet, uint64_t genesisAmount, IDebugSeeder* seeder,
		StackTestPortGetter* portSel, const File* projectFolder, ISystemLogger* logger) {
	CodablecashSystemParam param;
	DebugCodablecashSystemParamSetup::setupConfig02(param);

	CodablecashNetworkNodeConfig nwconfig;
	nwconfig.setSysConfig(&param);

	// host;
	UnicodeString localhost(L"localhost");
	nwconfig.setHost(&localhost);

	MultizoneTestnet* testnet = new MultizoneTestnet(portSel, projectFolder, logger, &param, &nwconfig);

	// genesis balance
	AddressDescriptor* addressDesc = wallet->getAddressDescriptor(0, 0); __STP(addressDesc);

	GenesisBalanceConfig gconfig;
	BalanceUnit amount(genesisAmount);
	gconfig.addBalance(amount, addressDesc);

	{
		TestnetInstanceWrapper* inst = testnet->createInstance(0, L"first");
		// genesis
		inst->setGenesisBlockConfig(&gconfig);

		// mining
		MiningConfig minerConfig;
		AddressDescriptor* desc2 = wallet->getAddressDescriptor(0, 1); __STP(desc2);
		minerConfig.setAddressDescriptor(desc2);
		inst->setMinerConfig(&minerConfig);

		// finalizer
		FinalizerConfig fconfig;
		NodeIdentifierSource* voteSource = NodeIdentifierSource::create(); __STP(voteSource);
		fconfig.setVoterSource(voteSource);
		inst->setFinalizerConfig(&fconfig);
	}

	testnet->startGenesis(0, seeder);
	testnet->waitForBlockHeight(0, 0, 2);

	{
		TestnetInstanceWrapper* inst = testnet->createInstance(0, L"second");
		testnet->startInstancesFrom(0, 1, seeder);
	}

	testnet->waitForBlockHeight(0, 1, 4);

	return testnet;
}

} /* namespace codablecash */
