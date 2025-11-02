/*
 * test_bloomfilter1024.cpp
 *
 *  Created on: Jun 17, 2025
 *      Author: iizuka
 */
#include "test_utils/t_macros.h"

#include "bc_wallet_filter/BloomFilter1024.h"
#include "bc_wallet_filter/BloomHash1024.h"

#include "bc_wallet/HdWalletSeed.h"

#include "ecda/ScPrivateKey.h"
#include "ecda/ScPublicKey.h"

#include "bc_base/BalanceAddress.h"
#include "bc_base/AddressDescriptor.h"


using namespace codablecash;


TEST_GROUP(TestBloomFilter1024Group) {
	TEST_SETUP(){
	}
	TEST_TEARDOWN(){
	}
};

TEST(TestBloomFilter1024Group, case01){
	BloomFilter1024 filter;
	ArrayList<BalanceAddress> list;
	list.setDeleteOnExit();

	ArrayList<BloomHash1024> hashlist;
	hashlist.setDeleteOnExit();

	int maxLoop = 256;
	for(int i = 0; i != maxLoop; ++i){
		HdWalletSeed* seed = HdWalletSeed::newSeed(); __STP(seed);
		BigInteger* bigInt = seed->toBigInteger(); __STP(bigInt);

		ScPrivateKey* privateKey = new ScPrivateKey(bigInt, 1); __STP(privateKey);
		ScPublicKey publicKey = privateKey->generatePublicKey();

		BalanceAddress* address = BalanceAddress::createAddress(1, &publicKey);
		list.addElement(address);

		AddressDescriptor* desc = address->toAddressDescriptor(); __STP(desc);

		BloomHash1024* hash = filter.getHash(desc); __STP(hash);
		hashlist.addElement(__STP_MV(hash));

		filter.add(address);
	}

	// check
	for(int i = 0; i != maxLoop; ++i){
		BalanceAddress* address = list.get(i);

		bool bl = filter.checkBytes(address);
		CHECK(bl == true);

		AddressDescriptor* desc = address->toAddressDescriptor(); __STP(desc);
		BloomHash1024* h = filter.getHash(desc); __STP(h);

		BloomHash1024* hash = hashlist.get(i);

		bl = h->equals(hash);
		CHECK(bl == true);

		bl = filter.checkBytes(hash);
		CHECK(bl == true);
	}

	for(int i = 0; i != maxLoop; ++i){
		BloomHash1024* hash = hashlist.get(i);

		bool bl = filter.checkBytes(hash);
		CHECK(bl == true);
	}

}

