/*
 * test_wallet.cpp
 *
 *  Created on: 2023/01/22
 *      Author: iizuka
 */


#include "test_utils/t_macros.h"

#include "bc_wallet/HdWallet.h"
#include "bc_wallet/HdWalletSeed.h"

#include "base/StackRelease.h"
#include "base/UnicodeString.h"

#include "bc_wallet_encoder/PasswordEncoder.h"


using namespace codablecash;

TEST_GROUP(TestWalletGroup) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};

TEST(TestWalletGroup, case01){
	HdWalletSeed* seed = HdWalletSeed::newSeed(); __STP(seed);
	HdWalletSeed* seed2 = dynamic_cast<HdWalletSeed*>(seed->copyData()); __STP(seed2);

	bool bl = seed->equals(seed2);
	CHECK(bl);
}

TEST(TestWalletGroup, case02){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"wallet"))

	HdWalletSeed* seed = HdWalletSeed::newSeed(); __STP(seed);

	UnicodeString pass(L"changeit");
	PasswordEncoder enc(&pass);
	HdWallet* wallet = HdWallet::create(baseDir, seed, 0, &enc); __STP(wallet);

	IWalletDataEncoder* enc2 = enc.copy(); __STP(enc2);

}
