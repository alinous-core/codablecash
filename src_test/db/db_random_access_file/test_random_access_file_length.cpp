/*
 * test_random_access_file_length.cpp
 *
 *  Created on: 2023/05/13
 *      Author: iizuka
 */


#include "test_utils/t_macros.h"

#include "random_access_file/RandomAccessFile.h"
#include "random_access_file/DiskCacheManager.h"
#include "base_io_stream/exceptions.h"

#include "random_access_file/MMapSegment.h"

using namespace alinous;


TEST_GROUP(RAFLengthTestGroup) {
	TEST_SETUP() {
		env->setup();
	}
	TEST_TEARDOWN() {
		env->teardown();
	}
};

TEST(RAFLengthTestGroup, setLength01){
	File projectFolder = this->env->testCaseDir();
	ErrorPointManager* errmgr = ErrorPointManager::getInstance();

	UnicodeString name(L"out.bin");
	File* outFile = projectFolder.get(&name);
	StackRelease<File> r_outFile(outFile);

	DiskCacheManager diskCache(16525);
	RandomAccessFile file(outFile, &diskCache);

	file.open();

	uint64_t newLength = 1024 * 20;
	file.setLength(newLength);

	file.close();
	CHECK(diskCache.size() == 0)
}

