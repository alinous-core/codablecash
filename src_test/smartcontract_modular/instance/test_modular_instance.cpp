/*
 * test_modular_instance.cpp
 *
 *  Created on: Sep 19, 2025
 *      Author: iizuka
 */
#include "test_utils/t_macros.h"




TEST_GROUP(TestModularInstanceGroup) {
	TEST_SETUP() {
		env->setup();
	}
	TEST_TEARDOWN() {
		env->teardown();
	}
};


TEST(TestModularInstanceGroup, case01){
	File projectFolder = this->env->testCaseDir();


}


