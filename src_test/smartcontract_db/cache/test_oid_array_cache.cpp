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


	cache.add(OidArrayCache::INDEX_ELEMENT_SIZE + 3, &oid);
}



