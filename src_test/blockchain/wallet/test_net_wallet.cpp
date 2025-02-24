/*
 * test_net_wallet.cpp
 *
 *  Created on: Feb 14, 2025
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"

#include "base_io/File.h"

#include "bc_wallet_net/NetworkWallet.h"

#include "bc_wallet/HdWalletSeed.h"

#include "bc_network_instance/INetworkSeeder.h"

using namespace codablecash;

class TesGrouptParams {
public:
	TesGrouptParams(){
		this->rootSeed = nullptr;
		this->seeder = nullptr;
	}
	~TesGrouptParams(){
		delete this->rootSeed;
		delete this->seeder;
	}

	HdWalletSeed* rootSeed;
	INetworkSeeder* seeder;
};

TEST_GROUP(TestNetWalletGroup) {
	TEST_SETUP(){
		this->params = new TesGrouptParams();
		this->env->getTestGroup()->setParam(this->params);

		File projectFolder = this->env->testCaseDir();
		_ST(File, baseDir, projectFolder.get(L"wallet_before"))

		UnicodeString pass(L"changeit");
		NetworkWallet* wallet = NetworkWallet::createNewWallet(baseDir, &pass, 0); __STP(wallet);

		this->params->rootSeed = wallet->getRootSeed();


	}
	TEST_TEARDOWN(){
		delete this->params;
	}

public:
	TesGrouptParams* params;
};


TEST(TestNetWalletGroup, case01){
	TesGrouptParams* params = (TesGrouptParams*)(this->env->getTestGroup()->getParam());
	const HdWalletSeed* rootSeed = params->rootSeed;
	INetworkSeeder* seeder = params->seeder;

	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"wallet"))

	UnicodeString pass(L"changeit");
	NetworkWallet* wallet = NetworkWallet::resotreWallet(baseDir, &pass, 0, rootSeed); __STP(wallet);
	wallet->setNetworkSeeder(seeder);


	// FIXME TestNetWalletGroup
}
