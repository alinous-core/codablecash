/*
 * test_unfinalized_repo.cpp
 *
 *  Created on: Jul 4, 2025
 *      Author: iizuka
 */
#include "test_utils/t_macros.h"

#include "bc_wallet/HdWalletSeed.h"
#include "bc_wallet/HdWallet.h"
#include "bc_wallet/WalletAccount.h"

#include "bc_wallet_encoder/PasswordEncoder.h"

#include "bc_wallet_net_data/HeaderTransactionGroup.h"

#include "bc_block/BlockHeaderId.h"

#include "bc_base/BalanceUnit.h"

#include "bc_trx_genesis/GenesisTransaction.h"
#include "bc_wallet_net_data/TransactionGroupDataStore.h"


using namespace codablecash;


TEST_GROUP(TestUnfinalizedRepoGroup) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};

TEST(TestUnfinalizedRepoGroup, case01) {
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"wallet"))
	_ST(File, basestoreDir, projectFolder.get(L"store"))

	HdWalletSeed* seed = HdWalletSeed::newSeed(); __STP(seed);

	UnicodeString pass(L"changeit");
	PasswordEncoder enc(&pass);
	HdWallet* wallet = HdWallet::create(baseDir, seed, 0, &enc, 10); __STP(wallet);
	WalletAccount* account = wallet->getAccount(0);

	{
		TransactionGroupDataStore store(basestoreDir);
		store.initBlank();
	}

	{
		TransactionGroupDataStore store(basestoreDir);
		store.open();

		char tmp[32];
		BlockHeaderId headerId(tmp, 32);

		HeaderTransactionGroup group;
		group.setHeaderId(&headerId);

		{
			BalanceUnit amount(100000000L);
			GenesisTransaction* trx = account->createGenesisTransaction(amount, 5); __STP(trx);
			group.addTransaction(trx);
		}

		store.add(&headerId, &group);

		{
			BalanceUnit amount(90000000L);
			GenesisTransaction* trx = account->createGenesisTransaction(amount, 1); __STP(trx);
			group.addTransaction(trx);
		}

		store.add(&headerId, &group);
	}
}

