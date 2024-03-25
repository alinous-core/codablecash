/*
 * test_register_vote_ticket.cpp
 *
 *  Created on: 2023/04/26
 *      Author: iizuka
 */
#include "test_utils/t_macros.h"

#include "../utils/InstanceDriver.h"
#include "bc/CodablecashConfig.h"

#include "bc_block_generator/MiningConfig.h"

#include "bc_base/BalanceUnit.h"

#include "../utils/DebugCodablecashConfigSetup.h"

using namespace codablecash;

TEST_GROUP(TestRegisterVoteTicketGroup) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};

TEST(TestRegisterVoteTicketGroup, case01){
	File projectFolder = this->env->testCaseDir();
	InstanceDriver driver(&projectFolder);

	CodablecashConfig config;
	DebugCodablecashConfigSetup::setupConfig01(config);

	driver.initWallet(1);
	driver.initInstance(&config);

	MiningConfig mconfig;
	driver.startMiner(&mconfig);

	driver.startFinalize();

	driver.initGenesisBalance(0, 10000000000);

	driver.registerVotingNode();

	{
		BalanceUnit stakeAmount(1000L);
		driver.registerTicket(stakeAmount);
	}

	driver.waitUntilHeight(2);
}
