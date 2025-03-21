/*
 * test_balance_trx.cpp
 *
 *  Created on: 2023/01/25
 *      Author: iizuka
 */


#include "test_utils/t_macros.h"

#include "bc_trx_balance/BalanceTransferTransaction.h"
#include "bc_trx_balance/BalanceUtxoReference.h"
#include "bc_trx_balance/BalanceUtxo.h"

#include "bc_wallet/HdWalletSeed.h"
#include "bc_wallet/HdWallet.h"
#include "bc_wallet/WalletAccount.h"

#include "bc_wallet_encoder/PasswordEncoder.h"

#include "bc_base/AddressDescriptor.h"

#include "bc_trx_genesis/GenesisTransaction.h"

#include "bc_trx/TransactionId.h"

#include "../wallet_util/WalletDriver.h"

#include "bc_base_trx_index/TransactionData.h"

#include "bc_trx_balance/FeeShortageException.h"
#include "bc_trx_balance/BalanceShortageException.h"

using namespace codablecash;

TEST_GROUP(TestBalanceTrxGroup) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};

TEST(TestBalanceTrxGroup, BalanceUtxo01_err){
	BalanceUnit amount(10L);
	BalanceUtxo utxo(amount);

	FeeShortageException* ex = nullptr;
	try{
		BalanceUnit feeRemain(100L);
		utxo.discountFee(feeRemain);
	}
	catch(FeeShortageException* e){
		ex = e;
	}
	CHECK(ex != nullptr);
	delete ex;
}

TEST(TestBalanceTrxGroup, case07){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"wallet"))

	WalletDriver driver(0, baseDir, L"changeit");
	driver.init(2);
	driver.initGenesis(0, 1000);

	BalanceShortageException* ex = nullptr;
	try{
		bool res;
		res = driver.transfer(0, 1, 1, BalanceUnit(9000), BalanceUnit(1), false);
	}
	catch(BalanceShortageException* e){
		ex = e;
	}
	CHECK(ex != nullptr);
	delete ex;
}


TEST(TestBalanceTrxGroup, checkexception01){
	testException<BalanceShortageException>();
}

TEST(TestBalanceTrxGroup, case06){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"wallet"))

	WalletDriver driver(0, baseDir, L"changeit");
	driver.init(2);
	driver.initGenesis(0, 1000);

	BalanceShortageException* ex = nullptr;
	try{
		bool res;
		res = driver.transfer(0, 1, 1, BalanceUnit(900), BalanceUnit(1), false);
	}
	catch(BalanceShortageException* e){
		ex = e;
	}
	CHECK(ex != nullptr);
	delete ex;
}

TEST(TestBalanceTrxGroup, case05){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"wallet"))

	WalletDriver driver(0, baseDir, L"changeit");
	driver.init(2);
	driver.initGenesis(0, 10000000);

	bool res;
	res = driver.transfer(0, 1, 1, BalanceUnit(5000), BalanceUnit(1), false);
	CHECK(res);
	res = driver.transfer(0, 1, 1, BalanceUnit(8000), BalanceUnit(1), false);
	CHECK(res);

	res = driver.transfer(1, 0, 1, BalanceUnit(2000), BalanceUnit(1), false);
	CHECK(res);
	res = driver.transfer(1, 0, 1, BalanceUnit(2000), BalanceUnit(1), false);
	CHECK(res);
}

TEST(TestBalanceTrxGroup, case04){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"wallet"))

	WalletDriver driver(0, baseDir, L"changeit");
	driver.init(2);
	driver.initGenesis(0, 10000000);

	bool res;
	res = driver.transfer(0, 1, 1, BalanceUnit(5000), BalanceUnit(1), false);
	CHECK(res);

	res = driver.transfer(0, 1, 1, BalanceUnit(8000), BalanceUnit(1), false);
	CHECK(res);

	res = driver.transfer(0, 1, 1, BalanceUnit(1000), BalanceUnit(1), false);
	CHECK(res);

	res = driver.transfer(0, 1, 1, BalanceUnit(500), BalanceUnit(1), false);
	CHECK(res);

	res = driver.transfer(0, 1, 1, BalanceUnit(499), BalanceUnit(1), false);
	CHECK(res);

	res = driver.transfer(0, 1, 1, BalanceUnit(498), BalanceUnit(1), false);
	CHECK(res);
	res = driver.transfer(0, 1, 1, BalanceUnit(497), BalanceUnit(1), false);
	CHECK(res);
	res = driver.transfer(0, 1, 1, BalanceUnit(496), BalanceUnit(1), false);
	CHECK(res);
	res = driver.transfer(0, 1, 1, BalanceUnit(495), BalanceUnit(1), false);
	CHECK(res);
	res = driver.transfer(0, 1, 1, BalanceUnit(493), BalanceUnit(1), false);
	CHECK(res);
	res = driver.transfer(0, 1, 1, BalanceUnit(492), BalanceUnit(1), false);
	CHECK(res);
	res = driver.transfer(0, 1, 1, BalanceUnit(491), BalanceUnit(1), false);
	CHECK(res);
	res = driver.transfer(0, 1, 1, BalanceUnit(490), BalanceUnit(1), false);
	CHECK(res);
}

TEST(TestBalanceTrxGroup, case03){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"wallet"))

	WalletDriver driver(0, baseDir, L"changeit");
	driver.init(2);
	driver.initGenesis(0, 10000000);

	bool res;
	res = driver.transfer(0, 1, 1, BalanceUnit(5000), BalanceUnit(1), false);
	CHECK(res);

	res = driver.transfer(0, 1, 1, BalanceUnit(8000), BalanceUnit(1), false);
	CHECK(res);

	BalanceUnit total = driver.getTotalBalance(1);
	CHECK(total.getAmount() == 13000);
}

TEST(TestBalanceTrxGroup, case02){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"wallet"))

	UnicodeString pass(L"changeit");
	PasswordEncoder enc(&pass);

	{
		HdWalletSeed* seed = HdWalletSeed::newSeed(); __STP(seed);
		HdWallet* wallet = HdWallet::create(baseDir, seed, 0, &enc, 10); __STP(wallet);
		WalletAccount* account = wallet->getAccount(0);
		BalanceUnit amount(100000000L);
		GenesisTransaction* trx = account->createGenesisTransaction(amount, 0); __STP(trx);
		account->importTransaction(trx);
	}

	{
		HdWallet* wallet = HdWallet::loadWallet(baseDir, &enc); __STP(wallet);
		WalletAccount* account = wallet->getAccount(0);

		AddressDescriptor* desc = account->getReceivingAddressDescriptor(3); __STP(desc);
		BalanceTransferTransaction* trx = account->createBalanceTransferTransaction(desc, BalanceUnit(5000), BalanceUnit(1), false, &enc); __STP(trx);

		bool vresult = trx->verify();
		CHECK(vresult);

		BalanceUnit rate = trx->getFeeRate();
		CHECK(rate.getAmount() == 1);

		account->importTransaction(trx);

		{
			AbstractBlockchainTransaction* t = account->findTransaction(trx->getTransactionId()); __STP(t);
			CHECK(t != nullptr);
		}

		{
			account->removeTransaction(trx->getTransactionId());
			AbstractBlockchainTransaction* t = account->findTransaction(trx->getTransactionId()); __STP(t);
			CHECK(t == nullptr);
		}

		TransactionData data(trx);
		TransactionData* data2 = dynamic_cast<TransactionData*>(data.copyData()); __STP(data2);
		CHECK(data2->getTrx()->getTransactionId()->compareTo(trx->getTransactionId()) == 0);
	}
}

TEST(TestBalanceTrxGroup, case01){
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

		BalanceUnit rate = trx->getFeeRate();
		CHECK(rate.getAmount() == 1);

		account->importTransaction(trx);

		{
			int size = trx->binarySize();
			ByteBuffer* buff = ByteBuffer::allocateWithEndian(size, true); __STP(buff);
			trx->toBinary(buff);

			buff->position(0);
			AbstractBlockchainTransaction* trx2 = AbstractBlockchainTransaction::createFromBinary(buff); __STP(trx2);
			const TransactionId* id2 = trx2->getTransactionId();
			const TransactionId* id = trx->getTransactionId();

			bool bl = id->equals(id2);
			CHECK(bl == true);
		}

		{
			BalanceTransferTransaction* trx2 = dynamic_cast<BalanceTransferTransaction*>(trx->copyData()); __STP(trx2);
			const TransactionId* id2 = trx2->getTransactionId();
			const TransactionId* id = trx->getTransactionId();

			bool bl = id->equals(id2);
			CHECK(bl == true);
		}
	}
}
