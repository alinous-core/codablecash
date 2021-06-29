/*
 * test_oid_array_cache.cpp
 *
 *  Created on: 2021/06/15
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"

#include "base/StackRelease.h"

#include "vm/VirtualMachine.h"

#include "base_io/File.h"

#include "random_access_file/DiskCacheManager.h"

#include "trx/transaction_cache_array/OidArrayCache.h"
#include "trx/transaction_cache_array/OidArrayCacheScanner.h"

#include "schema_table/record/table_record_local/LocalCdbOid.h"


TEST_GROUP(TestOidArrayCacheGroup) {
	TEST_SETUP() {
		env->setup();
	}
	TEST_TEARDOWN() {
		env->teardown();
	}
};


TEST(TestOidArrayCacheGroup, case01){
	UnicodeString fileName(L"tmp_cache");
	File testCaseFolder = this->env->testCaseDir();
	File* tmpDir = testCaseFolder.get(&fileName); __STP(tmpDir);

	UnicodeString* dir = testCaseFolder.getAbsolutePath(); __STP(dir);

	DiskCacheManager diskCache;

	OidArrayCache cache;
	cache.init(dir, &fileName, &diskCache);

	cache.shutdown();
}

TEST(TestOidArrayCacheGroup, case02){
	UnicodeString fileName(L"tmp_cache");
	File testCaseFolder = this->env->testCaseDir();
	File* tmpDir = testCaseFolder.get(&fileName); __STP(tmpDir);

	UnicodeString* dir = testCaseFolder.getAbsolutePath(); __STP(dir);

	DiskCacheManager diskCache;

	OidArrayCache cache;
	cache.init(dir, &fileName, &diskCache);

	LocalCdbOid oid(10);
	//cache.add(2, &oid);

	int index = OidArrayCache::DEFAULT_INDEX_ELEMENT_SIZE + 3;
	cache.add(index, &oid);

	OidArrayCacheScanner* scanner = cache.getScanner(index); __STP(scanner);
	while(scanner->hasNext()){
		const CdbOid* oid = scanner->next();

		uint64_t v = oid->getOidValue();
		CHECK(v == 10);
	}

}

TEST(TestOidArrayCacheGroup, case03){
	UnicodeString fileName(L"tmp_cache");
	File testCaseFolder = this->env->testCaseDir();
	File* tmpDir = testCaseFolder.get(&fileName); __STP(tmpDir);

	UnicodeString* dir = testCaseFolder.getAbsolutePath(); __STP(dir);

	DiskCacheManager diskCache;

	OidArrayCache cache(2, 2);
	cache.init(dir, &fileName, &diskCache);

	int maxLoop = 10;
	for(int i = 0; i != maxLoop; ++i){
		LocalCdbOid oid(i + 1);
		cache.add(i, &oid);
	}

	for(int i = 0; i != maxLoop; ++i){
		OidArrayCacheScanner* scanner = cache.getScanner(i); __STP(scanner);
		while(scanner->hasNext()){
			const CdbOid* oid = scanner->next();

			uint64_t v = oid->getOidValue();
			CHECK(v == i + 1);
		}
	}
}

TEST(TestOidArrayCacheGroup, case04){
	UnicodeString fileName(L"tmp_cache");
	File testCaseFolder = this->env->testCaseDir();
	File* tmpDir = testCaseFolder.get(&fileName); __STP(tmpDir);

	UnicodeString* dir = testCaseFolder.getAbsolutePath(); __STP(dir);

	DiskCacheManager diskCache;

	OidArrayCache cache(2, 2);
	cache.init(dir, &fileName, &diskCache);

	int arindex = 3;
	int maxLoop = 10;
	for(int i = 0; i != maxLoop; ++i){
		LocalCdbOid oid(i + 1);
		cache.add(arindex, &oid);
	}

	int value = 1;
	OidArrayCacheScanner* scanner = cache.getScanner(arindex); __STP(scanner);
	while(scanner->hasNext()){
		const CdbOid* oid = scanner->next();

		uint64_t v = oid->getOidValue();
		CHECK(v == value);
		value++;
	}
}

