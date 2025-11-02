/*
 * test_header_store.cpp
 *
 *  Created on: 2023/01/26
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"

#include "bc_blockstore_header/BlockHeaderStoreManager.h"

#include "bc_block/BlockHeader.h"

#include "base/ArrayList.h"

#include "bc_block/BlockHeaderId.h"
using namespace codablecash;

TEST_GROUP(TestBlockHeaderStoreGroup) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};

TEST(TestBlockHeaderStoreGroup, case01){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"store"))

	BlockHeaderStoreManager* manager = new BlockHeaderStoreManager(baseDir, 2); __STP(manager);

	BlockHeader header;
	header.setHeight(2);
	header.buildHeaderId();

	manager->addHeader(&header);

	ArrayList<BlockHeader>* list = manager->getBlocksAtHeight(2); __STP(list);
	list->setDeleteOnExit();
	CHECK(list->size() == 1);

	BlockHeader* header2 = list->get(0);
	const BlockHeaderId* id = header2->getId();

	bool bl = id->equals(header.getId());
	CHECK(bl);
}

TEST(TestBlockHeaderStoreGroup, case02){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"store"))

	BlockHeaderStoreManager* manager = new BlockHeaderStoreManager(baseDir, 2); __STP(manager);

	{
		BlockHeader header;
		header.setHeight(2);
		header.buildHeaderId();

		manager->addHeader(&header);
	}

	BlockHeaderId* id = nullptr;
	{
		BlockHeader header;
		header.setHeight(2);
		header.incTimestamp();
		header.buildHeaderId();
		id = dynamic_cast<BlockHeaderId*>(header.getId()->copyData());

		manager->addHeader(&header);
	}
	__STP(id);

	{
		ArrayList<BlockHeader>* list = manager->getBlocksAtHeight(2); __STP(list);
		list->setDeleteOnExit();
		CHECK(list->size() == 2);
	}

	manager->removeHeader(id, 2);

	{
		ArrayList<BlockHeader>* list = manager->getBlocksAtHeight(2); __STP(list);
		list->setDeleteOnExit();
		CHECK(list->size() == 1);
	}
}

TEST(TestBlockHeaderStoreGroup, cacheout01){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"store"))

	BlockHeaderStoreManager* manager = new BlockHeaderStoreManager(baseDir, 2); __STP(manager);

	{
		BlockHeader header;
		header.setHeight(2);
		header.buildHeaderId();

		manager->addHeader(&header);
	}

	{
		BlockHeader header;
		header.setHeight(5);
		header.buildHeaderId();

		manager->addHeader(&header);
	}

	{
		BlockHeader header;
		header.setHeight(10);
		header.buildHeaderId();

		manager->addHeader(&header);
	}

	{
		BlockHeader header;
		header.setHeight(15);
		header.buildHeaderId();

		manager->addHeader(&header);
	}
}

TEST(TestBlockHeaderStoreGroup, remove01){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"store"))

	BlockHeaderStoreManager* manager = new BlockHeaderStoreManager(baseDir, 2); __STP(manager);

	BlockHeader header;
	header.setHeight(2);
	header.buildHeaderId();
	const BlockHeaderId* headerId = header.getId();

	manager->addHeader(&header);

	{
		manager->removeHeader(headerId, 2);
	}

	manager->removeHeader(headerId, 2);
	manager->removeHeader(headerId, 200);

	{
		ArrayList<BlockHeader>* list = manager->getBlocksAtHeight(2); __STP(list);
		CHECK(list == nullptr);
	}
}

