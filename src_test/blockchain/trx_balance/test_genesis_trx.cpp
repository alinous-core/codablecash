/*
 * test_genesis_trx.cpp
 *
 *  Created on: 2023/01/25
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"

#include "bc_trx_genesis/GenesisTransaction.h"
#include "bc_trx_balance/BalanceUtxo.h"

#include "base/StackRelease.h"

#include "base_io/ByteBuffer.h"

#include "bc_wallet/HdWalletSeed.h"
#include "bc_wallet/HdWallet.h"
#include "bc_wallet/WalletAccount.h"

#include "bc_wallet_encoder/PasswordEncoder.h"

#include "bc_base/AddressDescriptor.h"

#include "bc_trx/TransactionId.h"
using namespace codablecash;

TEST_GROUP(TestGenesisTrxGroup) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};

TEST(TestGenesisTrxGroup, case03){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"wallet"))

	HdWalletSeed* seed = HdWalletSeed::newSeed(); __STP(seed);
	UnicodeString pass(L"changeit");
	PasswordEncoder enc(&pass);

	{
		HdWallet* wallet = HdWallet::create(baseDir, seed, 0, &enc); __STP(wallet);
		WalletAccount* account = wallet->getAccount(0);

		BalanceUnit amount(1000L);
		GenesisTransaction* trx = account->createGenesisTransaction(amount, 5); __STP(trx);

		account->importTransaction(trx);
	}

	{
		HdWallet* wallet = HdWallet::loadWallet(baseDir, &enc); __STP(wallet);
		WalletAccount* account = wallet->getAccount(0);

		BalanceUnit balance = account->getTotalAmount();
		BalanceUnit other(1000);
		CHECK(balance.compareTo(&other) == 0);
	}
}

TEST(TestGenesisTrxGroup, case02){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"wallet"))

	HdWalletSeed* seed = HdWalletSeed::newSeed(); __STP(seed);

	UnicodeString pass(L"changeit");
	PasswordEncoder enc(&pass);
	HdWallet* wallet = HdWallet::create(baseDir, seed, 0, &enc); __STP(wallet);
	WalletAccount* account = wallet->getAccount(0);

	BalanceUnit amount(1000L);
	GenesisTransaction* trx = account->createGenesisTransaction(amount, 5); __STP(trx);

	account->importTransaction(trx);

	BalanceUnit balance = account->getTotalAmount();
	BalanceUnit other(1000);
	CHECK(balance.compareTo(&other) == 0);

}

TEST(TestGenesisTrxGroup, case01){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"wallet"))

	HdWalletSeed* seed = HdWalletSeed::newSeed(); __STP(seed);

	UnicodeString pass(L"changeit");
	PasswordEncoder enc(&pass);
	HdWallet* wallet = HdWallet::create(baseDir, seed, 0, &enc); __STP(wallet);
	WalletAccount* account = wallet->getAccount(0);

	AddressDescriptor* addressDesc = account->getReceivingAddressDescriptor(0); __STP(addressDesc);

	GenesisTransaction* trx = new GenesisTransaction(); __STP(trx);

	BalanceUtxo utxo(BalanceUnit(100));
	utxo.setAddress(addressDesc);

	trx->addBalanceUtxo(&utxo);
	trx->build();

	GenesisTransaction* trx2 = dynamic_cast<GenesisTransaction*>(trx->copyData()); __STP(trx2);

	{
		int cap = trx2->binarySize();
		ByteBuffer* buff = ByteBuffer::allocateWithEndian(cap, true); __STP(buff);
		trx2->toBinary(buff);

		CHECK(buff->position() == buff->limit());

		buff->position(0);

		AbstractBlockchainTransaction* __trx3 = AbstractBlockchainTransaction::createFromBinary(buff); __STP(__trx3);
		GenesisTransaction* trx3 = dynamic_cast<GenesisTransaction*>(__trx3);

		BalanceUnit total = trx3->getTotalBalance();
		CHECK(utxo.getAmount().compareTo(&total) == 0);

		CHECK(trx3->getUtxoSize() == trx->getUtxoSize());

		BalanceUtxo* u = dynamic_cast<BalanceUtxo*>(trx3->getUtxo(0));
		const AddressDescriptor* addr = u->getAddress();
		CHECK(0 == addr->compareTo(addressDesc));

		AddressDescriptor* addr2 = dynamic_cast<AddressDescriptor*>(addr->copyData()); __STP(addr2);
		CHECK(0 == addr2->compareTo(addressDesc));

		const TransactionId* id = trx->getTransactionId();
		const TransactionId* id3 = trx3->getTransactionId();

		CHECK(id->equals(id3));
	}
}

