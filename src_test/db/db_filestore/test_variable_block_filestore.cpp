/*
 * test_variable_block_filestore.cpp
 *
 *  Created on: 2023/02/08
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

TEST_GROUP(TestVariableBlockFileStoreGroup) {
	TEST_SETUP() {
		env->setup();
	}
	TEST_TEARDOWN() {
		env->teardown();
	}
};

TEST(TestVariableBlockFileStoreGroup, case01){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"store"))
	_ST(UnicodeString, baseDirStr, baseDir->getAbsolutePath())

	DiskCacheManager cacheManager;
	UnicodeString name(L"file01");

	VariableBlockFileStore store(baseDirStr, &name, &cacheManager);

	store.createStore(true, 256, 32);
}

TEST(TestVariableBlockFileStoreGroup, case02){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"store"))
	_ST(UnicodeString, baseDirStr, baseDir->getAbsolutePath())

	DiskCacheManager cacheManager;
	UnicodeString name(L"file01");
	{
		VariableBlockFileStore store(baseDirStr, &name, &cacheManager);
		store.createStore(true, 256, 32);
	}

	{
		VariableBlockFileStore store(baseDirStr, &name, &cacheManager);
		store.open(false);

		store.close();
	}
}

TEST(TestVariableBlockFileStoreGroup, alloc01){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"store"))
	_ST(UnicodeString, baseDirStr, baseDir->getAbsolutePath())

	DiskCacheManager cacheManager;
	UnicodeString name(L"file01");
	{
		VariableBlockFileStore store(baseDirStr, &name, &cacheManager);
		store.createStore(true, 256, 32);
	}

	uint64_t fpos;
	{
		VariableBlockFileStore store(baseDirStr, &name, &cacheManager);
		store.open(false);

		const char* data = makeTestData(3, 10);
		StackArrayRelease<const char> _st_data(data);

		IBlockHandle* handle = store.alloc(10); __STP(handle);
		handle->write(data, 10);

		fpos = handle->getFpos();

		store.close();
	}

	{
		VariableBlockFileStore store(baseDirStr, &name, &cacheManager);
		store.open(false);

		IBlockHandle* handle = store.get(fpos); __STP(handle);
		ByteBuffer* buff = handle->getBuffer();

		bool bl = checkTestData(3, (const char*)buff->array(), 10);
		CHECK(bl);
		CHECK(buff->limit() == 10);

		store.close();
	}
}

/**
 * Allocate with extending file
 */
TEST(TestVariableBlockFileStoreGroup, alloc02){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"store"))
	_ST(UnicodeString, baseDirStr, baseDir->getAbsolutePath())

	DiskCacheManager cacheManager;
	UnicodeString name(L"file01");
	{
		VariableBlockFileStore store(baseDirStr, &name, &cacheManager);
		store.createStore(true, 256, 32);
	}

	uint64_t fpos;
	{
		VariableBlockFileStore store(baseDirStr, &name, &cacheManager);
		store.open(false);

		IBlockHandle* handle = store.alloc(300); __STP(handle);
		int size = handle->size();
		CHECK(size == 300);

		const char* data = makeTestData(3, 300);
		StackArrayRelease<const char> _st_data(data);

		handle->write(data, 300);
		fpos = handle->getFpos();

		store.close();
	}

	{
		VariableBlockFileStore store(baseDirStr, &name, &cacheManager);
		store.open(false);

		IBlockHandle* handle = store.get(fpos); __STP(handle);
		ByteBuffer* buff = handle->getBuffer();

		bool bl = checkTestData(3, (const char*)buff->array(), 300);
		CHECK(bl);
		CHECK(buff->limit() == 300);

		store.close();
	}
}

/**
 * large data than allocated
 **/
TEST(TestVariableBlockFileStoreGroup, alloc03){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"store"))
	_ST(UnicodeString, baseDirStr, baseDir->getAbsolutePath())

	DiskCacheManager cacheManager;
	UnicodeString name(L"file01");
	{
		VariableBlockFileStore store(baseDirStr, &name, &cacheManager);
		store.createStore(true, 256, 32);
	}

	uint64_t fpos;
	{
		VariableBlockFileStore store(baseDirStr, &name, &cacheManager);
		store.open(false);

		const char* data = makeTestData(3, 100);
		StackArrayRelease<const char> _st_data(data);

		IBlockHandle* handle = store.alloc(10); __STP(handle);
		handle->write(data, 100);

		fpos = handle->getFpos();

		store.close();
	}

	{
		VariableBlockFileStore store(baseDirStr, &name, &cacheManager);
		store.open(false);

		IBlockHandle* handle = store.get(fpos); __STP(handle);
		ByteBuffer* buff = handle->getBuffer();

		bool bl = checkTestData(3, (const char*)buff->array(), 100);
		CHECK(bl);
		CHECK(buff->limit() == 100);

		store.close();
	}
}

/**
 * fragment is 2
 */
TEST(TestVariableBlockFileStoreGroup, alloc04){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"store"))
	_ST(UnicodeString, baseDirStr, baseDir->getAbsolutePath())

	DiskCacheManager cacheManager;
	UnicodeString name(L"file01");
	{
		VariableBlockFileStore store(baseDirStr, &name, &cacheManager);
		store.createStore(true, 256, 32);
	}

	uint64_t fpos01, fpos02;
	{
		VariableBlockFileStore store(baseDirStr, &name, &cacheManager);
		store.open(false);

		{
			const char* data = makeTestData(3, 10);
			StackArrayRelease<const char> _st_data(data);
			IBlockHandle* handle = store.alloc(10); __STP(handle);
			handle->write(data, 10);
			fpos01 = handle->getFpos();
		}

		{
			const char* data = makeTestData(3, 10);
			StackArrayRelease<const char> _st_data(data);
			IBlockHandle* handle = store.alloc(10); __STP(handle);
			handle->write(data, 10);
			fpos02 = handle->getFpos();
		}

		{
			IBlockHandle* handle = store.get(fpos01); __STP(handle);
			handle->removeBlocks();
		}

		{
			const char* data = makeTestData(3, 100);
			StackArrayRelease<const char> _st_data(data);

			IBlockHandle* handle = store.alloc(100); __STP(handle);
			handle->write(data, 100);
			fpos01 = handle->getFpos();
		}

		store.close();
	}

	{
		VariableBlockFileStore store(baseDirStr, &name, &cacheManager);
		store.open(false);

		{
			IBlockHandle* handle = store.get(fpos01); __STP(handle);
			ByteBuffer* buff = handle->getBuffer();

			bool bl = checkTestData(3, (const char*)buff->array(), 100);
			CHECK(bl);
			CHECK(buff->limit() == 100);
		}

		store.close();
	}
}

/**
 * fragment is 2 with realloc & extend
 */
TEST(TestVariableBlockFileStoreGroup, alloc05){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"store"))
	_ST(UnicodeString, baseDirStr, baseDir->getAbsolutePath())

	DiskCacheManager cacheManager;
	UnicodeString name(L"file01");
	{
		VariableBlockFileStore store(baseDirStr, &name, &cacheManager);
		store.createStore(true, 256, 32);
	}

	uint64_t fpos01, fpos02;
	{
		VariableBlockFileStore store(baseDirStr, &name, &cacheManager);
		store.open(false);

		{
			const char* data = makeTestData(3, 10);
			StackArrayRelease<const char> _st_data(data);
			IBlockHandle* handle = store.alloc(10); __STP(handle);
			handle->write(data, 10);
			fpos01 = handle->getFpos();
		}

		{
			const char* data = makeTestData(3, 10);
			StackArrayRelease<const char> _st_data(data);
			IBlockHandle* handle = store.alloc(10); __STP(handle);
			handle->write(data, 10);
			fpos02 = handle->getFpos();
		}

		{
			IBlockHandle* handle = store.get(fpos01); __STP(handle);
			handle->removeBlocks();
		}

		{
			const char* data = makeTestData(3, 1000);
			StackArrayRelease<const char> _st_data(data);

			IBlockHandle* handle = store.alloc(10); __STP(handle);
			handle->write(data, 1000);
			fpos01 = handle->getFpos();
		}

		store.close();
	}

	{
		VariableBlockFileStore store(baseDirStr, &name, &cacheManager);
		store.open(false);

		{
			IBlockHandle* handle = store.get(fpos01); __STP(handle);
			ByteBuffer* buff = handle->getBuffer();

			bool bl = checkTestData(3, (const char*)buff->array(), 1000);
			CHECK(bl);
			CHECK(buff->limit() == 1000);
		}

		store.close();
	}
}

TEST(TestVariableBlockFileStoreGroup, remove01){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"store"))
	_ST(UnicodeString, baseDirStr, baseDir->getAbsolutePath())

	DiskCacheManager cacheManager;
	UnicodeString name(L"file01");
	{
		VariableBlockFileStore store(baseDirStr, &name, &cacheManager);
		store.createStore(true, 256, 32);
	}

	uint64_t fpos;
	{
		VariableBlockFileStore store(baseDirStr, &name, &cacheManager);
		store.open(false);

		const char* data = makeTestData(3, 10);
		StackArrayRelease<const char> _st_data(data);

		IBlockHandle* handle = store.alloc(10); __STP(handle);
		handle->write(data, 10);

		fpos = handle->getFpos();

		store.close();
	}

	{
		VariableBlockFileStore store(baseDirStr, &name, &cacheManager);
		store.open(false);

		IBlockHandle* handle = store.get(fpos); __STP(handle);
		handle->removeBlocks();

		store.close();
	}
}

