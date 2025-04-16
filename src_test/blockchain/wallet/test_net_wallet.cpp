/*
 * test_net_wallet.cpp
 *
 *  Created on: Feb 14, 2025
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"

#include "base_io/File.h"

#include "bc_base/AddressDescriptor.h"

#include "bc_wallet_net/NetworkWallet.h"

#include "bc_wallet/HdWalletSeed.h"

#include "bc_network_instance/INetworkSeeder.h"
#include "../utils_testnet/MultizoneTestnet.h"
#include "../utils_testnet/ArrayDebugSeeder.h"
#include "../utils_testnet/IDebugSeeder.h"
#include "../utils_testnet/TestnetInstanceWrapper.h"
#include "../../test_utils/TestPortSelection.h"

#include "pubsub/CommandPublisherSetting.h"

#include "bc/DebugDefaultLogger.h"
#include "bc/CodablecashSystemParam.h"

#include "../utils/DebugCodablecashSystemParamSetup.h"

#include "bc_base/BalanceUnit.h"

#include "bc_block_generator/MiningConfig.h"

#include "bc_network_instance/FinalizerConfig.h"
#include "bc_network_instance/GenesisBalanceConfig.h"
#include "bc_network_instance/CodablecashNetworkNodeConfig.h"

#include "bc_network/NodeIdentifierSource.h"

#include "bc_wallet_encoder/PasswordEncoder.h"

#include "setup/TestnetSetupper01.h"

using namespace codablecash;

class TesGrouptParams {
public:
	TesGrouptParams(){
		this->rootSeed = nullptr;
		this->seeder = new ArrayDebugSeeder();
		this->logger = new DebugDefaultLogger();
		this->config = nullptr;
	}
	~TesGrouptParams(){
		delete this->rootSeed;
		delete this->seeder;
		delete this->logger;
		delete this->config;
	}

	HdWalletSeed* rootSeed;
	IDebugSeeder* seeder;
	DebugDefaultLogger* logger;
	CodablecashSystemParam *config;
};

TEST_GROUP(TestNetWalletGroup) {
	TEST_SETUP(){
		env->setup();

		CommandPublisherSettingStack __set(5, 300);

		this->portSel = new StackTestPortGetter();
		this->params = new TesGrouptParams();
		this->params->logger->setSection(ISystemLogger::DEBUG_NODE_TRANSFER_RESPONSE);

		DebugDefaultLogger* logger = this->params->logger;

		this->env->getTestGroup()->setParam(this->params);

		File projectFolder = this->env->testCaseDir();
		_ST(File, baseDir, projectFolder.get(L"wallet_before"))

		UnicodeString pass(L"changeit");
		PasswordEncoder enc(&pass);



		CodablecashSystemParam param;
		DebugCodablecashSystemParamSetup::setupConfig02(param);

		NetworkWallet* wallet = NetworkWallet::createNewWallet(baseDir, &pass, 0, 10, logger, &param); __STP(wallet);
		this->params->rootSeed = wallet->getRootSeed(&enc);
		this->params->config = new CodablecashSystemParam(param);

		this->testnet = TestnetSetupper01::setup2NodeZone0(wallet, 1000L * 10000L * 10000L, params->seeder, this->portSel, &projectFolder, params->logger);
	}
	TEST_TEARDOWN(){
		delete this->testnet;
		delete this->params;
		delete this->portSel;

		env->teardown();
	}

public:
	TesGrouptParams* params;
	StackTestPortGetter* portSel;
	MultizoneTestnet* testnet;
};


TEST(TestNetWalletGroup, case01){
	TesGrouptParams* params = (TesGrouptParams*)(this->env->getTestGroup()->getParam());
	const HdWalletSeed* rootSeed = params->rootSeed;
	INetworkSeeder* seeder = params->seeder;
	const CodablecashSystemParam* config = params->config;

	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"wallet"))

	DebugDefaultLogger* logger = params->logger;

	UnicodeString pass(L"changeit");
	NetworkWallet* wallet = NetworkWallet::resotreWallet(baseDir, &pass, 0, rootSeed, 10, logger, config); __STP(wallet);

	PasswordEncoder enc(&pass);
	wallet->initNetwork(seeder, &enc);

	// sync
	wallet->startNetwork();

	// register pool


	// FIXME TestNetWalletGroup
}
