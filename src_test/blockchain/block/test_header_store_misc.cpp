/*
 * test_header_store_misc.cpp
 *
 *  Created on: 2023/02/16
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"

#include "bc_blockstore_header/BlockHeaderStoreManager.h"

#include "bc_block/BlockHeader.h"

#include "base/ArrayList.h"

#include "bc_block/BlockHeaderId.h"

#include "bc_blockstore_header/BlockHeaderList.h"

#include "bc_blockstore_header/BlockHeaderHeightIndexKey.h"

#include "btreekey/InfinityKey.h"
#include "btreekey/NullKey.h"

#include "bc_block/BlockMerkleRootKey.h"

#include "bc_blockstore_body/FilePositionData.h"

#include "bc_blockstore_header/BlockHeaderIdKey.h"
using namespace codablecash;

TEST_GROUP(TestBlockHeaderStoreMiscGroup) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};

TEST(TestBlockHeaderStoreMiscGroup, BlockHeaderList01){
	BlockHeaderList list;
	list.add(10);

	int ret = list.indexof(11);
	CHECK(ret < 0)

	IBlockObject* obj = list.copyData(); __STP(obj);
	BlockHeaderList* list2 = dynamic_cast<BlockHeaderList*>(obj);

	CHECK(list2->size() == 1);

	list.join(list2);
	CHECK(list.size() == 1);
}

TEST(TestBlockHeaderStoreMiscGroup, key01){
	BlockHeaderHeightIndexKey key(10L);
	InfinityKey sinf;
	NullKey nullkey;

	CHECK(key.compareTo(&sinf) < 0);
	CHECK(key.compareTo(&nullkey) > 0);
}

TEST(TestBlockHeaderStoreMiscGroup, headerCppy01){
	BlockHeader header;
	header.setHeight(2);
	header.buildHeaderId();

	BlockHeader* header2 = dynamic_cast<BlockHeader*>(header.copyData()); __STP(header2);
	BlockHeaderId* id2 = header2->getId();

	CHECK(id2->equals(header.getId()));
}

TEST(TestBlockHeaderStoreMiscGroup, key02){
	BlockMerkleRootKey key(nullptr);
	InfinityKey sinf;
	NullKey nullkey;

	CHECK(key.compareTo(&sinf) < 0);
	CHECK(key.compareTo(&nullkey) > 0);
}

TEST(TestBlockHeaderStoreMiscGroup, FilePositionData02){
	FilePositionData data(64);
	FilePositionData* data2 = dynamic_cast<FilePositionData*>(data.copyData()); __STP(data2);

	CHECK(data.getFpos() == data2->getFpos());
}

TEST(TestBlockHeaderStoreMiscGroup, BlockHeaderIdKey02){
	char tmp[32];
	BlockHeaderId id(tmp, 32);
	BlockHeaderIdKey key(&id);

	NullKey nkey;
	InfinityKey infKey;

	CHECK(key.compareTo(&nkey) > 0);
	CHECK(key.compareTo(&infKey) < 0);
}

