/*
 * test_wallet_mempool_index.cpp
 *
 *  Created on: Jun 10, 2026
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"

#include "bc_wallet_net_data/WalletMemoryPool.h"
#include "bc_wallet_net_data/WalletTransactionIdListDataFactory.h"

#include "base_timestamp/SystemTimestampKeyFactory.h"


#include "../wallet_util/WalletDriver.h"
#include "btree/BtreeConfig.h"
#include "btree/Btree.h"

#include "random_access_file/DiskCacheManager.h"

#include "bc_trx/TransactionId.h"

#include "base_timestamp/SystemTimestampKey.h"

#include "bc_wallet_net_data/WalletTransactionIdListData.h"

#include "base_timestamp/SystemTimestamp.h"
using namespace codablecash;


TEST_GROUP(TestWalletMempoolIndexGroup) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};

TEST(TestWalletMempoolIndexGroup, case01){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"poolindex"))

	{
		UnicodeString fileName(WalletMemoryPool::NAME_TRX_ORDER_INDEX);
		DiskCacheManager cacheManager;

		SystemTimestampKeyFactory* keyFactory = new SystemTimestampKeyFactory(); __STP(keyFactory);
		WalletTransactionIdListDataFactory* dataFactory = new WalletTransactionIdListDataFactory(); __STP(dataFactory);

		Btree btree(baseDir, &fileName, &cacheManager, keyFactory, dataFactory);

		BtreeConfig config;
		config.nodeNumber = 8;
		config.defaultSize = 1024;
		config.blockSize = 32;
		btree.create(&config);
	}

	{
		UnicodeString fileName(WalletMemoryPool::NAME_TRX_ORDER_INDEX);
		DiskCacheManager cacheManager;

		SystemTimestampKeyFactory* keyFactory = new SystemTimestampKeyFactory(); __STP(keyFactory);
		WalletTransactionIdListDataFactory* dataFactory = new WalletTransactionIdListDataFactory(); __STP(dataFactory);

		Btree* trxOrderIndex = new Btree(baseDir, &fileName, &cacheManager, keyFactory, dataFactory); __STP(trxOrderIndex);
		BtreeOpenConfig opconf;
		opconf.numDataBuffer = 256;
		opconf.numNodeBuffer = 512;
		trxOrderIndex->open(&opconf);

		TransactionId* trx01 = TransactionId::makeRandomTransactionId(); __STP(trx01);
		TransactionId* trx02 = TransactionId::makeRandomTransactionId(); __STP(trx02);

		SystemTimestamp tm01;

		// add
		{
			SystemTimestampKey key(&tm01);
			WalletTransactionIdListData data;
			data.add(trx01);

			trxOrderIndex->putData(&key, &data);
		}
		{
			SystemTimestampKey key(&tm01);
			WalletTransactionIdListData data;
			data.add(trx02);

			trxOrderIndex->putData(&key, &data);
		}
		{
			SystemTimestampKey key(&tm01);
			WalletTransactionIdListData data;
			data.add(trx02);

			trxOrderIndex->putData(&key, &data);
		}

		// remove
		{
			SystemTimestampKey key(&tm01);
			key.setRemoveKey(trx02);
			trxOrderIndex->remove(&key);
		}

		trxOrderIndex->close();
	}
}


