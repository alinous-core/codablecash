/*
 * test_address.cpp
 *
 *  Created on: 2023/01/23
 *      Author: iizuka
 */
#include "test_utils/t_macros.h"

#include "numeric/BigInteger.h"

#include "base_io/ByteBuffer.h"

#include "base/StackRelease.h"

#include "bc_wallet/HdWalletSeed.h"

#include "ecda/ScPrivateKey.h"
#include "ecda/ScPublicKey.h"

#include "bc_base/BalanceAddress.h"
#include "bc_base/AddressDescriptor.h"

#include "bc_base_utxo_index/AddressDescriptorUtxoData.h"

#include "bc_trx/AbstractUtxo.h"

#include "bc_trx_balance/BalanceUtxo.h"
using alinous::StackArrayRelease;

using namespace codablecash;

TEST_GROUP(TestWalletAddressGroup) {
	TEST_SETUP(){
	}
	TEST_TEARDOWN(){
	}
};

TEST(TestWalletAddressGroup, bigIntPadding){
	BigInteger bigint(12345);

	int size = bigint.binarySize();
	ByteBuffer* buff = bigint.toBinary(); __STP(buff);

	buff->position(0);
	ByteBuffer* newbuff = BigInteger::padBuffer(buff, 32); __STP(newbuff);

	newbuff->position(0);
	BigInteger* num = BigInteger::fromBinary((const char*)newbuff->array(), newbuff->limit()); __STP(num);

	int64_t v = num->longValue();
	CHECK(v == 12345);
}

TEST(TestWalletAddressGroup, case01){
	HdWalletSeed* seed = HdWalletSeed::newSeed(); __STP(seed);
	BigInteger* bigInt = seed->toBigInteger(); __STP(bigInt);

	ScPrivateKey* privateKey = new ScPrivateKey(bigInt, 1); __STP(privateKey);
	ScPublicKey publicKey = privateKey->generatePublicKey();

	BalanceAddress* address = BalanceAddress::createAddress(1, &publicKey); __STP(address);

	AddressDescriptor* desc = address->toAddressDescriptor(); __STP(desc);

	const char* straddress = desc->toCString();
	StackArrayRelease<const char> __st_straddress(straddress);

	AddressDescriptor* desc2 = new AddressDescriptor(straddress); __STP(desc2);

	UnicodeString str(straddress);
	AddressDescriptor* desc3 = new AddressDescriptor(&str); __STP(desc3);

	int diff = desc->compareTo(desc2);
	CHECK(diff == 0);

	diff = desc->compareTo(desc3);
	CHECK(diff == 0);
}

TEST(TestWalletAddressGroup, AddressDescriptorUtxoData01){
	HdWalletSeed* seed = HdWalletSeed::newSeed(); __STP(seed);
	BigInteger* bigInt = seed->toBigInteger(); __STP(bigInt);

	ScPrivateKey* privateKey = new ScPrivateKey(bigInt, 1); __STP(privateKey);
	ScPublicKey publicKey = privateKey->generatePublicKey();

	BalanceAddress* address = BalanceAddress::createAddress(1, &publicKey); __STP(address);

	AddressDescriptor* desc = address->toAddressDescriptor(); __STP(desc);


	BalanceUtxo u(BalanceUnit(10));
	u.setAddress(desc);
	u.build();

	AddressDescriptorUtxoData data;

	int index = data.indexof(&u);
	CHECK(index < 0);

	data.add(&u);

	AddressDescriptorUtxoData* data2 = dynamic_cast<AddressDescriptorUtxoData*>(data.copyData()); __STP(data2);
	data.join(data2);
}


