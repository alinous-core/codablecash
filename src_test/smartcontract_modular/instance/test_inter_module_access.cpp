/*
 * test_inter_module_access.cpp
 *
 *  Created on: Dec 3, 2025
 *      Author: iizuka
 */
#include "test_utils/t_macros.h"

TEST_GROUP(TestInterModuleAccessGroup) {
	TEST_SETUP() {
		env->setup();
	}
	TEST_TEARDOWN() {
		env->teardown();
	}
};

TEST(TestInterModuleAccessGroup, case01){

}

