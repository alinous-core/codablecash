/*
 * test_wallet_misc.cpp
 *
 *  Created on: 2023/02/25
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"

#include "bc_wallet/HdWalletSeed.h"

#include "bc_wallet_encoder/PasswordEncoder.h"

#include "bc_base/Base64.h"
#include "bc_base/BinaryUtils.h"

#include "crypto/Sha256.h"

#include "base/Exception.h"

#include "bc_trx_balance/FeeShortageException.h"

#include "bc_trx/AbstractBlockchainTransaction.h"

#include "bc_trx/AbstractUtxo.h"

#include "bc_trx/AbstractUtxoReference.h"

#include "bc_base_utxo_index/AddressDescriptorKey.h"

#include "btreekey/InfinityKey.h"

#include "btreekey/NullKey.h"

#include "bc_base_utxo_index/UtxoIdKey.h"

#include "bc_base_trx_index/TransactionIdKey.h"

using namespace codablecash;

TEST_GROUP(TestWalletMiscGroup) {
	TEST_SETUP(){}
	TEST_TEARDOWN(){}
};

TEST(TestWalletMiscGroup, binaryError01){
	Exception* ex = nullptr;
	try{
		BinaryUtils::checkNotNull(nullptr);
	}
	catch(Exception* e){
		ex = e;
	}
	CHECK(ex != nullptr);
	delete ex;
}

TEST(TestWalletMiscGroup, binaryError02){
	Exception* ex = nullptr;
	try{
		BinaryUtils::checkUShortRange(100, 1, 3);
	}
	catch(Exception* e){
		ex = e;
	}
	CHECK(ex != nullptr);
	delete ex;
}

TEST(TestWalletMiscGroup, binaryError03){
	Exception* ex = nullptr;
	try{
		BinaryUtils::checkUint8Value(1, 3);
	}
	catch(Exception* e){
		ex = e;
	}
	CHECK(ex != nullptr);
	delete ex;
}

TEST(TestWalletMiscGroup, balanceShortageEx){
	testException<FeeShortageException>();
}

TEST(TestWalletMiscGroup, createFromBinary01){
	ByteBuffer* in = ByteBuffer::allocate(10); __STP(in);

	AbstractBlockchainTransaction* trx = AbstractBlockchainTransaction::createFromBinary(in);
	CHECK(trx == nullptr);
}

TEST(TestWalletMiscGroup, createFromBinary02){
	ByteBuffer* in = ByteBuffer::allocate(10); __STP(in);

	AbstractUtxo* utxo = AbstractUtxo::createFromBinary(in);
	CHECK(utxo == nullptr);
}

TEST(TestWalletMiscGroup, createFromBinary03){
	ByteBuffer* in = ByteBuffer::allocate(10); __STP(in);

	AbstractUtxoReference* ref = AbstractUtxoReference::createFromBinary(in);
	CHECK(ref == nullptr);
}

TEST(TestWalletMiscGroup, AddressDescriptorKey01){
	AddressDescriptorKey key;
	InfinityKey sinf;
	NullKey nullkey;

	CHECK(key.compareTo(&sinf) < 0);
	CHECK(key.compareTo(&nullkey) > 0);
}

TEST(TestWalletMiscGroup, UtxoIdKey01){
	UtxoIdKey key;
	InfinityKey sinf;
	NullKey nullkey;

	CHECK(key.compareTo(&sinf) < 0);
	CHECK(key.compareTo(&nullkey) > 0);
}

TEST(TestWalletMiscGroup, TransactionIdKey01){
	TransactionIdKey key;
	InfinityKey sinf;
	NullKey nullkey;

	CHECK(key.compareTo(&sinf) < 0);
	CHECK(key.compareTo(&nullkey) > 0);
}
