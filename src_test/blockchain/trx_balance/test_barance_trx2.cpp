/*
 * test_barance_trx2.cpp
 *
 *  Created on: May 24, 2025
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"

#include "bc_wallet/HdWalletSeed.h"
#include "bc_wallet/HdWallet.h"
#include "bc_wallet/WalletAccount.h"

#include "bc_wallet_encoder/PasswordEncoder.h"

#include "bc_base/BalanceUnit.h"
#include "bc_base/AddressDescriptor.h"

#include "bc_trx_genesis/GenesisTransaction.h"

#include "bc_trx_balance/BalanceTransferTransaction.h"

#include "bc_wallet_trx_base/HdWalletAccountTrxBuilderContext.h"
#include "bc_wallet_trx_base/IUtxoCollector.h"

using namespace codablecash;

TEST_GROUP(TestBalanceTrx2Group) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};

static bool balancecheck(BalanceTransferTransaction* trx, BalanceUnit& total){
	BalanceUnit right = trx->getFee();
	{
		int maxLoop = trx->getUtxoSize();
		for(int i = 0; i != maxLoop; ++i){
			AbstractUtxo* utxo = trx->getUtxo(i);
			BalanceUnit unit = utxo->getAmount();

			right += unit;
		}
	}

	return total.compareTo(right) == 0;
}


TEST(TestBalanceTrx2Group, case01){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"wallet"))

	UnicodeString pass(L"changeit");
	PasswordEncoder enc(&pass);

	{
		HdWalletSeed* seed = HdWalletSeed::newSeed(); __STP(seed);
		HdWallet* wallet = HdWallet::create(baseDir, seed, 0, &enc, 10); __STP(wallet);
		WalletAccount* account = wallet->getAccount(0);
		BalanceUnit amount(100000000L);
		GenesisTransaction* trx = account->createGenesisTransaction(amount, 5); __STP(trx);
		account->importTransaction(trx);
	}

	{
		HdWallet* wallet = HdWallet::loadWallet(baseDir, &enc); __STP(wallet);
		WalletAccount* account = wallet->getAccount(0);

		AddressDescriptor* desc = account->getReceivingAddressDescriptor(3); __STP(desc);
		BalanceTransferTransaction* trx = account->createBalanceTransferTransaction(desc, BalanceUnit(5000), BalanceUnit(1), false, &enc); __STP(trx);

		bool vresult = trx->verify();
		CHECK(vresult);

		BalanceUnit amount(100000000L);
		vresult = balancecheck(trx, amount);
		CHECK(vresult);
	}
}

TEST(TestBalanceTrx2Group, case02){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"wallet"))

	UnicodeString pass(L"changeit");
	PasswordEncoder enc(&pass);

	{
		HdWalletSeed* seed = HdWalletSeed::newSeed(); __STP(seed);
		HdWallet* wallet = HdWallet::create(baseDir, seed, 0, &enc, 10); __STP(wallet);
		WalletAccount* account = wallet->getAccount(0);

		{
			BalanceUnit amount(1250L);
			GenesisTransaction* trx = account->createGenesisTransaction(amount, 5); __STP(trx);
			account->importTransaction(trx);
		}
		{
			BalanceUnit amount(1000L);
			GenesisTransaction* trx = account->createGenesisTransaction(amount, 5); __STP(trx);
			account->importTransaction(trx);
		}

	}

	{
		HdWallet* wallet = HdWallet::loadWallet(baseDir, &enc); __STP(wallet);
		WalletAccount* account = wallet->getAccount(0);

		AddressDescriptor* desc = account->getReceivingAddressDescriptor(3); __STP(desc);
		BalanceTransferTransaction* trx = account->createBalanceTransferTransaction(desc, BalanceUnit(1000L), BalanceUnit(1L), false, &enc); __STP(trx);

		bool vresult = trx->verify();
		CHECK(vresult);

		BalanceUnit amount(1250L + 1000L);
		vresult = balancecheck(trx, amount);
		CHECK(vresult);

		account->importTransaction(trx);
	}

	// collector test
	{
		HdWallet* wallet = HdWallet::loadWallet(baseDir, &enc); __STP(wallet);
		WalletAccount* account = wallet->getAccount(0);

		HdWalletAccountTrxBuilderContext context(account, &enc);

		IUtxoCollector* collector = context.getUtxoCollector(); __STP(collector);

		while(collector->hasNext()){
			BalanceUtxo* utxo = collector->next(); __STP(utxo);

		}

	}
}


TEST(TestBalanceTrx2Group, caseInclude01){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"wallet"))

	UnicodeString pass(L"changeit");
	PasswordEncoder enc(&pass);

	{
		HdWalletSeed* seed = HdWalletSeed::newSeed(); __STP(seed);
		HdWallet* wallet = HdWallet::create(baseDir, seed, 0, &enc, 10); __STP(wallet);
		WalletAccount* account = wallet->getAccount(0);
		BalanceUnit amount(100000000L);
		GenesisTransaction* trx = account->createGenesisTransaction(amount, 5); __STP(trx);
		account->importTransaction(trx);
	}

	{
		HdWallet* wallet = HdWallet::loadWallet(baseDir, &enc); __STP(wallet);
		WalletAccount* account = wallet->getAccount(0);

		AddressDescriptor* desc = account->getReceivingAddressDescriptor(3); __STP(desc);
		BalanceTransferTransaction* trx = account->createBalanceTransferTransaction(desc, BalanceUnit(5000), BalanceUnit(1), true, &enc); __STP(trx);

		bool vresult = trx->verify();
		CHECK(vresult);
	}
}
