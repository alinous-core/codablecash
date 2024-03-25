/*
 * test_sync_header_only.cpp
 *
 *  Created on: 2023/12/12
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"

TEST_GROUP(TestSyncHeaderOnlyGroup) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};

TEST(TestSyncHeaderOnlyGroup, case01){

}

