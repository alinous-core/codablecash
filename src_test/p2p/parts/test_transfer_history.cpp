/*
 * test_transfer_history.cpp
 *
 *  Created on: 2023/09/13
 *      Author: iizuka
 */


#include "test_utils/t_macros.h"

#include "data_history/DataHistoryStore.h"

using namespace codablecash;


TEST_GROUP(TestTransferHistoryGroup) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};

TEST(TestTransferHistoryGroup, case01){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"codable_home"))

	DataHistoryStore store(baseDir);
}


