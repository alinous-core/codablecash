/*
 * test_record_identity_cache.cpp
 *
 *  Created on: 2022/01/28
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"

#include "base/StackRelease.h"

#include "random_access_file/DiskCacheManager.h"

#include "trx/transaction_cache/CdbSwapCacheFactory.h"
#include "trx/transaction_cache/RecordScanIdentityCache.h"

#include "schema_table/record/table_record_local/LocalOidFactory.h"
#include "schema_table/record/table_record_local/LocalCdbOid.h"

#include "schema_table/record/table_record/CdbRecord.h"

#include "schema_table/record/table_record_value/CdbIntValue.h"


using namespace codablecash;

TEST_GROUP(TestRecordIdentityCacheGroup) {
	TEST_SETUP() {
		env->setup();
	}
	TEST_TEARDOWN() {
		env->teardown();
	}
};

TEST(TestRecordIdentityCacheGroup, case01){
	File testCaseFolder = this->env->testCaseDir();
	File* tmpDir = testCaseFolder.get(L"tmp_cache"); __STP(tmpDir);

	DiskCacheManager diskCache;

	CdbSwapCacheFactory factory(tmpDir, &diskCache);
	factory.resetDir();

	RecordScanIdentityCache* cache = factory.createRecordScanIdentityCache(1024); __STP(cache);
	CHECK(cache != nullptr)

	{
		LocalOidFactory factory;

		CdbRecord rec;
		rec.addValue(new CdbIntValue(1));

		LocalCdbOid* loid = factory.createLocalOid(); __STP(loid);

		rec.setOid(loid);
		rec.importScanIdentity();

		ScanRecordIndentity* id = rec.getScanIdentity();

		cache->insert(id);

		bool bl = cache->hasKey(id);
		CHECK(bl);
	}
}

