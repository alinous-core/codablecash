/*
 * test_btree_remove_add.cpp
 *
 *  Created on: 2023/05/13
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"

#include "random_access_file/DiskCacheManager.h"

#include "btreekey/BtreeKeyFactory.h"

#include "btree/Btree.h"
#include "btree/BtreeConfig.h"

#include "btreekey/ULongKey.h"
#include "TempValue.h"

using namespace alinous;

TEST_GROUP(TestBTreeRemoveAddGroup) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};

static void addKeyValue(uint64_t key, uint64_t value, Btree* btree){
	ULongKey lkey(key);
	TempValue tvalue(value);

	btree->putData(&lkey, &tvalue);
}

TEST(TestBTreeRemoveAddGroup, case01){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"store"))
	_ST(UnicodeString, baseDirStr, baseDir->getAbsolutePath())

	DiskCacheManager cacheManager;
	UnicodeString name(L"file01");
	BtreeKeyFactory* factory = new BtreeKeyFactory(); __STP(factory);
	TmpValueFactory* dfactory = new TmpValueFactory(); __STP(dfactory);

	Btree btree(baseDir, &name, &cacheManager, factory, dfactory);

	BtreeConfig config;
	config.nodeNumber  = 2;
	btree.create(&config);

	BtreeOpenConfig opconf;
	btree.open(&opconf);

	RawArrayPrimitive<uint64_t> answers(32);
	{
		addKeyValue(2, 2, &btree);
		addKeyValue(3, 3, &btree);
		addKeyValue(6, 6, &btree);
		addKeyValue(10, 10, &btree);
	}

	{
		ULongKey lkey(10);
		btree.remove(&lkey);
	}
	{
		ULongKey lkey(6);
		btree.remove(&lkey);
	}
	{
		ULongKey lkey(10);
		btree.remove(&lkey);
	}
	{
		ULongKey lkey(3);
		btree.remove(&lkey);
	}
	{
		ULongKey lkey(2);
		btree.remove(&lkey);
	}

	{
		ULongKey lkey(6);
		btree.findByKey(&lkey);
	}

	{
		addKeyValue(10, 10, &btree);
		ULongKey lkey(10);
		IBlockObject* obj = btree.findByKey(&lkey); __STP(obj);
	}

	btree.close();
}
