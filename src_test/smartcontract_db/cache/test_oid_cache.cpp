/*
 * test_oid_cache.cpp
 *
 *  Created on: 2020/09/24
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"

#include "base/StackRelease.h"
#include "base/RawArrayPrimitive.h"

#include "vm/VirtualMachine.h"

#include "random_access_file/DiskCacheManager.h"

#include "trx/transaction_cache/CdbSwapCacheFactory.h"
#include "trx/transaction_cache/SingleKeyOidCache.h"

#include "btree/IBtreeScanner.h"

#include "btreekey/ULongKey.h"

#include "engine/CdbOid.h"

#include "schema_table/record/table_record_key/CdbOidKey.h"

#include "schema_table/record/table_record_local/LocalCdbOid.h"

#include "schema_table/record/table_record_value/CdbOidValueList.h"
TEST_GROUP(TestOidCacheGroup) {
	TEST_SETUP() {
		env->setup();
	}
	TEST_TEARDOWN() {
		env->teardown();
	}
};

TEST(TestOidCacheGroup, case01){
	File testCaseFolder = this->env->testCaseDir();
	File* tmpDir = testCaseFolder.get(L"tmp_cache"); __STP(tmpDir);

	DiskCacheManager diskCache;

	CdbSwapCacheFactory factory(tmpDir, &diskCache);
	factory.resetDir();

	SingleKeyOidCache* cache = factory.createSingleKeyOidCache(1024); __STP(cache);

	cache->removeFiles();
}

TEST(TestOidCacheGroup, memory01){
	File testCaseFolder = this->env->testCaseDir();
	File* tmpDir = testCaseFolder.get(L"tmp_cache"); __STP(tmpDir);

	DiskCacheManager diskCache;

	CdbSwapCacheFactory factory(tmpDir, &diskCache);
	factory.resetDir();

	SingleKeyOidCache* cache = factory.createSingleKeyOidCache(1024); __STP(cache);

	CdbOid oid(10);
	CdbOidKey key(&oid);

	cache->insert(&key, &oid);

	bool bl = cache->hasKey(&key);
	CHECK(bl);

	cache->removeFiles();
}

TEST(TestOidCacheGroup, memory02){
	File testCaseFolder = this->env->testCaseDir();
	File* tmpDir = testCaseFolder.get(L"tmp_cache"); __STP(tmpDir);

	DiskCacheManager diskCache;

	CdbSwapCacheFactory factory(tmpDir, &diskCache);
	factory.resetDir();

	SingleKeyOidCache* cache = factory.createSingleKeyOidCache(1024); __STP(cache);

	CdbOid oid(10);
	CdbOidKey key(&oid);

	cache->insert(&key, &oid);
	cache->insert(&key, &oid);

	bool bl = cache->hasKey(&key);
	CHECK(bl);

	IBtreeScanner* scanner = cache->getScanner(); __STP(scanner);
	scanner->begin();

	while(scanner->hasNext()){
		const IBlockObject* obj = scanner->next();
		const AbstractBtreeKey* k = scanner->nextKey();

		const CdbOidKey* oidk = dynamic_cast<const CdbOidKey*>(k);
		int diff = key.compareTo(oidk);
		CHECK(diff == 0);
	}

	cache->removeFiles();
}

static void addKeyValue(RawArrayPrimitive<int64_t>* array, SingleKeyOidCache* cache, uint64_t value);
void addKeyValue(RawArrayPrimitive<int64_t>* array, SingleKeyOidCache* cache, uint64_t value){
	array->addElement(value);
	CdbOid oid(value);
	CdbOidKey key(&oid);

	cache->insert(&key, &oid);
}

TEST(TestOidCacheGroup, swap01){
	File testCaseFolder = this->env->testCaseDir();
	File* tmpDir = testCaseFolder.get(L"tmp_cache"); __STP(tmpDir);

	DiskCacheManager diskCache;

	CdbSwapCacheFactory factory(tmpDir, &diskCache);
	factory.resetDir();

	SingleKeyOidCache* cache = factory.createSingleKeyOidCache(50); __STP(cache);

	RawArrayPrimitive<int64_t> array(8);
	addKeyValue(&array, cache, 1);
	addKeyValue(&array, cache, 2);
	addKeyValue(&array, cache, 3);
	addKeyValue(&array, cache, 4);
	addKeyValue(&array, cache, 5);

	int maxLoop = array.size();
	for(int i = 0; i != maxLoop; ++i){
		int64_t value = array.get(i);

		CdbOid oid(value);
		CdbOidKey key(&oid);

		bool result = cache->hasKey(&key);
		CHECK(result)
	}

	IBtreeScanner* scanner = cache->getScanner(); __STP(scanner);
	scanner->begin();

	int i = 0;
	while(scanner->hasNext()){
		const IBlockObject* obj = scanner->next();
		const AbstractBtreeKey* k = scanner->nextKey();

		const CdbOidKey* oidk = dynamic_cast<const CdbOidKey*>(k);
		uint64_t v = oidk->getOid()->getOidValue();
		uint64_t v2 = array.get(i);

		CHECK(v == v2);

		++i;
	}
}

TEST(TestOidCacheGroup, swap02){ // remove
	File testCaseFolder = this->env->testCaseDir();
	File* tmpDir = testCaseFolder.get(L"tmp_cache"); __STP(tmpDir);

	DiskCacheManager diskCache;

	CdbSwapCacheFactory factory(tmpDir, &diskCache);
	factory.resetDir();

	SingleKeyOidCache* cache = factory.createSingleKeyOidCache(50); __STP(cache);

	RawArrayPrimitive<int64_t> array(8);
	addKeyValue(&array, cache, 1);
	addKeyValue(&array, cache, 2);
	addKeyValue(&array, cache, 3);
	addKeyValue(&array, cache, 4);
	addKeyValue(&array, cache, 5);

	int maxLoop = array.size();
	for(int i = 0; i != maxLoop; ++i){
		int64_t value = array.get(i);

		CdbOid oid(value);
		CdbOidKey key(&oid);

		bool result = cache->hasKey(&key);
		CHECK(result)
	}

	// remove
	CdbOid oid(3);
	CdbOidKey key(&oid);

	cache->remove(&key, &oid);

	const CdbOidValueList* ret = cache->findOids(&key);
	CHECK(ret == nullptr)
}

TEST(TestOidCacheGroup, swap03){ // remove 2
	File testCaseFolder = this->env->testCaseDir();
	File* tmpDir = testCaseFolder.get(L"tmp_cache"); __STP(tmpDir);

	DiskCacheManager diskCache;

	CdbSwapCacheFactory factory(tmpDir, &diskCache);
	factory.resetDir();

	SingleKeyOidCache* cache = factory.createSingleKeyOidCache(50); __STP(cache);

	RawArrayPrimitive<int64_t> array(8);
	addKeyValue(&array, cache, 1);
	addKeyValue(&array, cache, 2);
	addKeyValue(&array, cache, 3);
	addKeyValue(&array, cache, 4);
	addKeyValue(&array, cache, 5);

	int maxLoop = array.size();
	for(int i = 0; i != maxLoop; ++i){
		int64_t value = array.get(i);

		CdbOid oid(value);
		CdbOidKey key(&oid);

		bool result = cache->hasKey(&key);
		CHECK(result)
	}

	// remove after add
	CdbOid oid(3);
	CdbOidKey key(&oid);
	CdbOid oid2(300);

	cache->insert(&key, &oid2);

	cache->remove(&key, &oid);
	const CdbOidValueList* ret = cache->findOids(&key);
	CHECK(ret != nullptr)

	CHECK(ret->size() == 1);
}

TEST(TestOidCacheGroup, LocalOidKey01){
	File testCaseFolder = this->env->testCaseDir();
	File* tmpDir = testCaseFolder.get(L"tmp_cache"); __STP(tmpDir);

	DiskCacheManager diskCache;

	CdbSwapCacheFactory factory(tmpDir, &diskCache);
	factory.resetDir();

	SingleKeyOidCache* cache = factory.createSingleKeyOidCache(50); __STP(cache);

	uint64_t value = 15;

	LocalCdbOid oid(value);
	CdbOidKey key(&oid);

	cache->insert(&key, &oid);

	const CdbOidValueList* list = cache->findOids(&key);
	int s = list->size();
	CHECK(s == 1);

	CdbOid* retoid = list->get(0);

	bool isLocal = retoid->isLocal();
	CHECK(isLocal);
}

