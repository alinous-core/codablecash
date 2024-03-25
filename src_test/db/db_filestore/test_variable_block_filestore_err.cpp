/*
 * test_variable_block_filestore_err.cpp
 *
 *  Created on: 2023/02/10
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

#include "random_access_file/RandomAccessFile.h"
using namespace alinous;

TEST_GROUP(TestVariableBlockFileStoreErrGroup) {
	TEST_SETUP() {
		env->setup();
	}
	TEST_TEARDOWN() {
		env->teardown();
	}
};

TEST(TestVariableBlockFileStoreErrGroup, openStoreCreateError){
	ErrorPointManager* errmgr = ErrorPointManager::getInstance();
	errmgr->activatePoint(L"VariableBlockFileStore::createStore", L"Os::seekFile", 1);

	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"store"))
	_ST(UnicodeString, baseDirStr, baseDir->getAbsolutePath())

	DiskCacheManager cacheManager;
	UnicodeString name(L"file01");
	VariableBlockFileStore store(baseDirStr, &name, &cacheManager);

	Exception* exp = nullptr;
	try{
		store.createStore(false, 256, 32);
	}catch(Exception* e){
		exp = e;
	}

	CHECK(exp != nullptr)
	delete exp;
}

TEST(TestVariableBlockFileStoreErrGroup, openStoreError){
	ErrorPointManager* errmgr = ErrorPointManager::getInstance();
	errmgr->activatePoint(L"VariableBlockFileStore::open", L"Os::seekFile", 1);

	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"store"))
	_ST(UnicodeString, baseDirStr, baseDir->getAbsolutePath())

	DiskCacheManager cacheManager;
	UnicodeString name(L"file01");
	VariableBlockFileStore store(baseDirStr, &name, &cacheManager);

	store.createStore(false, 256, 32);

	Exception* exp = nullptr;
	try{
		store.open(false);
	}catch(Exception* e){
		exp = e;
	}

	CHECK(exp != nullptr)
	delete exp;
}

TEST(TestVariableBlockFileStoreErrGroup, headerError01){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"store"))
	_ST(UnicodeString, baseDirStr, baseDir->getAbsolutePath())

	DiskCacheManager cacheManager;
	UnicodeString name(L"file01");
	VariableBlockFileStore store(baseDirStr, &name, &cacheManager);

	store.createStore(false, 256, 32);

	{
		_ST(UnicodeString, headerfilename, new UnicodeString(name))
		headerfilename->append(L"-header.bin");
		_ST(File, storeHeaderFile, baseDir->get(headerfilename))
		RandomAccessFile* headerFile = new RandomAccessFile(storeHeaderFile, &cacheManager);

		headerFile->open(false);

		uint64_t zero = 0;
		headerFile->write(0, (char*)&zero, sizeof(uint64_t));
		headerFile->sync(true);
		headerFile->close();
		delete headerFile;
	}

	Exception* exp = nullptr;
	try{
		store.open(false);
	}catch(Exception* e){
		exp = e;
	}

	CHECK(exp != nullptr)
	delete exp;
}



