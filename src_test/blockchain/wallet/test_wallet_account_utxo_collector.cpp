/*
 * test_wallet_account_utx_collector.cpp
 *
 *  Created on: Jul 30, 2025
 *      Author: iizuka
 */
#include "test_utils/t_macros.h"

#include "../wallet_util/WalletDriver.h"
#include "bc_wallet_net_data_management/ManagementAccountsCollection.h"

#include "bc_wallet_net_data_management/ManagementAccount.h"

#include "bc_wallet/WalletAccount.h"

#include "bc_wallet_trx_base/NetWalletAccountUtxoCollector.h"
using namespace codablecash;


TEST_GROUP(TestWalletAccountUtxoCollecorGroup) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};


TEST(TestWalletAccountUtxoCollecorGroup, case01){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"pool"))
	_ST(File, walletBaseDir, projectFolder.get(L"wallet"))

	WalletDriver driver(0, walletBaseDir, L"changeit");
	driver.init(2);
	driver.initGenesis(0, 100000);

	WalletAccount* wa = driver.getAccount(1, 0);

	int amountcnt = 1;

	ManagementAccountsCollection managementAccounts;

	// finalized
	{
		ManagementAccount* account = managementAccounts.getFinalizedManagementAccount();
		int maxLoop = 3;
		for(int i = 0; i != maxLoop; ++i){
			BalanceTransferTransaction* trx = driver.createBalanceTransferTransaction(0, 1, 0, BalanceUnit(amountcnt), BalanceUnit(1), false); __STP(trx);
			account->addTransaction(trx, wa);

			driver.importTransaction(trx);
			amountcnt++;
		}
	}

	// unfinalized
	{
		ManagementAccount* account = managementAccounts.getUnFinalizedManagementAccount();
		ManagementAccount* faccount = managementAccounts.getFinalizedManagementAccount();
		account->importOtherAccount(faccount);

		int maxLoop = 3;
		for(int i = 0; i != maxLoop; ++i){
			BalanceTransferTransaction* trx = driver.createBalanceTransferTransaction(0, 1, 0, BalanceUnit(amountcnt), BalanceUnit(1), false); __STP(trx);
			account->addTransaction(trx, wa);
			amountcnt++;
		}
	}

	// mempool
	{
		ManagementAccount* account = managementAccounts.getMempoolManagementAccount();
		ManagementAccount* uaccount = managementAccounts.getUnFinalizedManagementAccount();
		account->importOtherAccount(uaccount);

		int maxLoop = 3;
		for(int i = 0; i != maxLoop; ++i){
			BalanceTransferTransaction* trx = driver.createBalanceTransferTransaction(0, 1, 0, BalanceUnit(amountcnt), BalanceUnit(1), false); __STP(trx);
			account->addTransaction(trx, wa);
			amountcnt++;
		}
	}

	ManagementAccount* managementAccount = managementAccounts.getMempoolManagementAccount();
	NetWalletAccountUtxoCollector* collector = new NetWalletAccountUtxoCollector(managementAccount); __STP(collector);
	collector->init();

	int count = 0;
	while(collector->hasNext()){
		BalanceUtxo* utxo = collector->next(); __STP(utxo);

		count++;
	}

	CHECK(count == 9);
}
