/*
 * test_bloomfilter.cpp
 *
 *  Created on: Mar 30, 2025
 *      Author: iizuka
 */
#include "test_utils/t_macros.h"

#include "bc_wallet_filter/BloomFilter512.h"

#include "bc_wallet/HdWalletSeed.h"

#include "ecda/ScPrivateKey.h"
#include "ecda/ScPublicKey.h"

#include "bc_base/BalanceAddress.h"

#include "base_io/ByteBuffer.h"

using namespace codablecash;

TEST_GROUP(TestBloomFilterGroup) {
	TEST_SETUP(){
	}
	TEST_TEARDOWN(){
	}
};

TEST(TestBloomFilterGroup, case01){
	BloomFilter512 filter;
	ArrayList<BalanceAddress> list;
	list.setDeleteOnExit();

	int maxLoop = 1024;
	for(int i = 0; i != maxLoop; ++i){
		HdWalletSeed* seed = HdWalletSeed::newSeed(); __STP(seed);
		BigInteger* bigInt = seed->toBigInteger(); __STP(bigInt);

		ScPrivateKey* privateKey = new ScPrivateKey(bigInt, 1); __STP(privateKey);
		ScPublicKey publicKey = privateKey->generatePublicKey();

		BalanceAddress* address = BalanceAddress::createAddress(1, &publicKey);
		list.addElement(address);
	}

	maxLoop = list.size();
	for(int i = 0; i != maxLoop; ++i){
		BalanceAddress* address = list.get(i);

		filter.add(address);
	}

	// check
	for(int i = 0; i != maxLoop; ++i){
		BalanceAddress* address = list.get(i);

		bool bl = filter.checkBytes(address);
		CHECK(bl == true);
	}
}

TEST(TestBloomFilterGroup, copy01) {
	BloomFilter512 filter;
	ArrayList<BalanceAddress> list;
	list.setDeleteOnExit();

	int maxLoop = 1024;
	for(int i = 0; i != maxLoop; ++i){
		HdWalletSeed* seed = HdWalletSeed::newSeed(); __STP(seed);
		BigInteger* bigInt = seed->toBigInteger(); __STP(bigInt);

		ScPrivateKey* privateKey = new ScPrivateKey(bigInt, 1); __STP(privateKey);
		ScPublicKey publicKey = privateKey->generatePublicKey();

		BalanceAddress* address = BalanceAddress::createAddress(1, &publicKey);
		list.addElement(address);

		filter.add(address);
	}

	// copy constructor
	{
		BloomFilter512* filter2 = dynamic_cast<BloomFilter512*>(filter.copyData()); __STP(filter2);
		bool bl = filter2->equals(&filter);
		CHECK(bl == true)

		// check
		for(int i = 0; i != maxLoop; ++i){
			BalanceAddress* address = list.get(i);

			bool bl = filter2->checkBytes(address);
			CHECK(bl == true);
		}
	}

	// binary functions
	{
		int total = filter.binarySize();
		ByteBuffer* buff = ByteBuffer::allocateWithEndian(total, true); __STP(buff);

		filter.toBinary(buff);
		buff->position(0);

		BloomFilter512* filter2 = BloomFilter512::createFromBinary(buff); __STP(filter2);

		// check
		for(int i = 0; i != maxLoop; ++i){
			BalanceAddress* address = list.get(i);

			bool bl = filter2->checkBytes(address);
			CHECK(bl == true);
		}
	}
}

TEST(TestBloomFilterGroup, copy02) {
	BloomFilter512 filter;
	ArrayList<BalanceAddress> list;
	list.setDeleteOnExit();

	int maxLoop = 10;
	for(int i = 0; i != maxLoop; ++i){
		HdWalletSeed* seed = HdWalletSeed::newSeed(); __STP(seed);
		BigInteger* bigInt = seed->toBigInteger(); __STP(bigInt);

		ScPrivateKey* privateKey = new ScPrivateKey(bigInt, 1); __STP(privateKey);
		ScPublicKey publicKey = privateKey->generatePublicKey();

		BalanceAddress* address = BalanceAddress::createAddress(1, &publicKey);
		list.addElement(address);

		filter.add(address);
	}

	// binary functions
	{
		int total = filter.binarySize();
		ByteBuffer* buff = ByteBuffer::allocateWithEndian(total, true); __STP(buff);

		filter.toBinary(buff);
		buff->position(0);

		BloomFilter512* filter2 = BloomFilter512::createFromBinary(buff); __STP(filter2);

		// check
		for(int i = 0; i != maxLoop; ++i){
			BalanceAddress* address = list.get(i);

			bool bl = filter2->checkBytes(address);
			CHECK(bl == true);
		}
	}
}
