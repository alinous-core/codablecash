/*
 * test_mempool.cpp
 *
 *  Created on: 2019/01/05
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"
#include "mempool/MemPool.h"
#include "mempool/TransactionRecord.h"

#include "base_io/ByteBuffer.h"
#include "base/StackRelease.h"

#include "bc_base/Transaction.h"
#include "bc_base/TransactionId.h"
#include "bc_base/BlockchainAddress.h"
#include "bc_base/BalanceUnit.h"
#include "bc_network/NetworkShard.h"
#include "bc_network/NetworkShardsStatus.h"

#include "blockchain/DummyTrxUtils.h"

#include "mempool_idx/FeeTransactionsListValue.h"
#include "mempool/TransactionIdKey.h"
#include "mempool_idx/FeeIndexKey.h"

#include "btreekey/InfinityKey.h"
#include "btreekey/NullKey.h"

using namespace alinous;
using namespace codablecash;

TEST_GROUP(TestMempoolGroup) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};

TEST(TestMempoolGroup, TransactionIdKey){
	NetworkShardsStatus* address = new NetworkShardsStatus(8);
	StackRelease<NetworkShardsStatus> __st_address(address);
	NetworkShard* shard = address->getShard(2);

	AbstractTransaction* trx = DummyTrxUtils::makeTrx(shard, 1000, 2, 3, 1);
	StackRelease<AbstractTransaction> __st_trx(trx);

	trx->updateTransactionId();
	const TransactionId* trxid = trx->getTransactionId();

	TransactionIdKey key(trxid);
	InfinityKey infkey;

	int res = key.compareTo(&infkey);
	CHECK(res < 0)

	NullKey nullkey;
	res = key.compareTo(&nullkey);
	CHECK(res > 0)
}

TEST(TestMempoolGroup, FeeIndexKey){
	FeeIndexKey key(10);
	InfinityKey infkey;

	int res = key.compareTo(&infkey);
	CHECK(res < 0)
}

TEST(TestMempoolGroup, FeeIndexKeyNull){
	FeeIndexKey key(10);
	NullKey nullkey;

	int res = key.compareTo(&nullkey);
	CHECK(res > 0)
}

TEST(TestMempoolGroup, FeeTransactionsListValueIndexof){
	FeeTransactionsListValue* baseValue = new FeeTransactionsListValue();
	StackRelease<FeeTransactionsListValue> __st_base(baseValue);

	int index = baseValue->indexof(10);
	CHECK(index < 0)
}


TEST(TestMempoolGroup, list){
	FeeTransactionsListValue list1(10);
	FeeTransactionsListValue list2(11);
	FeeTransactionsListValue list3(11);
	FeeTransactionsListValue list4;

	list1.join(&list2);
	list1.join(&list3);
	list1.join(&list4);

}

TEST(TestMempoolGroup, constract){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"store"))

	MemPool memPool(baseDir);
}

TEST(TestMempoolGroup, init){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"store"))

	{
		MemPool memPool(baseDir, 2048);
		memPool.init();
		memPool.close();
	}

	{
		MemPool memPool(baseDir);
		memPool.init();
		memPool.close();
	}
}

static void addTrx(MemPool* memPool, const NetworkShard* shard, uint64_t total, int numIn, int numOut, uint64_t fee) {
	AbstractTransaction* trx = DummyTrxUtils::makeTrx(shard, total, numIn, numOut, fee);
	StackRelease<AbstractTransaction> __st_trx(trx);

	memPool->addTransaction(trx);
}

TEST(TestMempoolGroup, addTrx){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"store"))

	NetworkShardsStatus* address = new NetworkShardsStatus(8);
	StackRelease<NetworkShardsStatus> __st_address(address);
	NetworkShard* shard = address->getShard(2);

	MemPool memPool(baseDir);
	memPool.init();

	addTrx(&memPool, shard, 1000, 2, 3, 1);
	addTrx(&memPool, shard, 1000, 2, 3, 1);
	addTrx(&memPool, shard, 1000, 2, 3, 1);
	addTrx(&memPool, shard, 1000, 2, 3, 1);
	addTrx(&memPool, shard, 1000, 2, 3, 1);
	addTrx(&memPool, shard, 1000, 2, 3, 1);
	addTrx(&memPool, shard, 1000, 2, 3, 1);
	addTrx(&memPool, shard, 1000, 2, 3, 1);
	addTrx(&memPool, shard, 1000, 2, 3, 1);
	addTrx(&memPool, shard, 1000, 2, 3, 1);
	addTrx(&memPool, shard, 1000, 2, 3, 1);
	addTrx(&memPool, shard, 1000, 2, 3, 1);

	addTrx(&memPool, shard, 1000, 2, 3, 2);
	addTrx(&memPool, shard, 1000, 2, 3, 2);
	addTrx(&memPool, shard, 1000, 2, 3, 2);
	addTrx(&memPool, shard, 1000, 2, 3, 2);
	addTrx(&memPool, shard, 1000, 2, 3, 2);
	addTrx(&memPool, shard, 1000, 2, 3, 2);
	addTrx(&memPool, shard, 1000, 2, 3, 2);

	addTrx(&memPool, shard, 1000, 2, 3, 2);
	addTrx(&memPool, shard, 1000, 2, 3, 3);
	addTrx(&memPool, shard, 1000, 2, 3, 4);
	addTrx(&memPool, shard, 1000, 2, 3, 5);
	addTrx(&memPool, shard, 1000, 2, 3, 6);
	addTrx(&memPool, shard, 1000, 2, 3, 7);
	addTrx(&memPool, shard, 1000, 2, 3, 8);
	addTrx(&memPool, shard, 1000, 2, 3, 9);
	addTrx(&memPool, shard, 1000, 2, 3, 10);

	{
		AbstractTransaction* trx = DummyTrxUtils::makeTrx(shard, 1000, 1, 1, 1);
		StackRelease<AbstractTransaction> __st_trx(trx);

		memPool.addTransaction(trx);

		trx->updateTransactionId();
		const TransactionId* trxid = trx->getTransactionId();
		TransactionRecord* rec = memPool.findByTransactionId(trxid);
		CHECK(rec != nullptr)
		delete rec;

		bool result = memPool.removeTransaction(trxid);
		CHECK(result)

		rec = memPool.findByTransactionId(trxid);
		CHECK(rec == nullptr)

		result = memPool.removeTransaction(trxid);
		CHECK(!result)
	}

	memPool.close();

}

TEST(TestMempoolGroup, removeAll){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"store"))

	NetworkShardsStatus* address = new NetworkShardsStatus(8);
	StackRelease<NetworkShardsStatus> __st_address(address);
	NetworkShard* shard = address->getShard(2);

	MemPool memPool(baseDir);
	memPool.init();

	{
		AbstractTransaction* trx = DummyTrxUtils::makeTrx(shard, 1000, 1, 1, 1);
		StackRelease<AbstractTransaction> __st_trx(trx);

		memPool.addTransaction(trx);

		trx->updateTransactionId();
		const TransactionId* trxid = trx->getTransactionId();
		TransactionRecord* rec = memPool.findByTransactionId(trxid);
		CHECK(rec != nullptr)
		delete rec;

		bool result = memPool.removeTransaction(trxid);
		CHECK(result)

		rec = memPool.findByTransactionId(trxid);
		CHECK(rec == nullptr)

		result = memPool.removeTransaction(trxid);
		CHECK(!result)
	}

	memPool.close();

}
