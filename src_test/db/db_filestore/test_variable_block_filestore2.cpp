/*
 * test_variable_block_filestore2.cpp
 *
 *  Created on: 2023/05/13
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"
#include "db_exceptions.h"

#include "random_access_file/DiskCacheManager.h"

#include "filestore_variable_block/VariableBlockFileStore.h"

#include "base_io/File.h"
#include "base/StackRelease.h"

#include "filestore_block/IBlockHandle.h"

#include "base_io/ByteBuffer.h"

#include "test_variable_block_data.h"

using namespace alinous;

TEST_GROUP(TestVariableBlockFileStore2Group) {
	TEST_SETUP() {
		env->setup();
	}
	TEST_TEARDOWN() {
		env->teardown();
	}
};

TEST(TestVariableBlockFileStore2Group, rewrite01){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"store"))
	_ST(UnicodeString, baseDirStr, baseDir->getAbsolutePath())

	DiskCacheManager cacheManager;
	UnicodeString name(L"file01");
	{
		VariableBlockFileStore store(baseDirStr, &name, &cacheManager);
		store.createStore(true, 256, 32);
	}

	uint64_t fpos, fpos2, fpos3, fpos4;
	{
		VariableBlockFileStore store(baseDirStr, &name, &cacheManager);
		store.open(false);

		{
			IBlockHandle* handle = store.alloc(10); __STP(handle);
			fpos = handle->getFpos();
		}
		{
			IBlockHandle* handle = store.alloc(10); __STP(handle);
			fpos2 = handle->getFpos();
		}
		{
			IBlockHandle* handle = store.alloc(10); __STP(handle);
			fpos3 = handle->getFpos();
		}
		{
			IBlockHandle* handle = store.alloc(10); __STP(handle);
			fpos4 = handle->getFpos();
		}

		// remove
		{
			IBlockHandle* handle = store.get(fpos2); __STP(handle);
			handle->removeBlocks();
		}
		{
			IBlockHandle* handle = store.get(fpos4); __STP(handle);
			handle->removeBlocks();
		}


		store.close();
	}

	{
		VariableBlockFileStore store(baseDirStr, &name, &cacheManager);
		store.open(false);

		int length = 1396;
		const char* data = makeTestData(3, length);
		StackArrayRelease<const char> _st_data(data);

		IBlockHandle* handle = store.get(fpos); __STP(handle);
		handle->write(data, length);

		CHECK(fpos == handle->getFpos());
		store.close();
	}
}


