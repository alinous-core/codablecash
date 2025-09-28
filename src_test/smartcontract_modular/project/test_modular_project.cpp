/*
 * test_modular_project.cpp
 *
 *  Created on: Sep 19, 2025
 *      Author: iizuka
 */
#include "test_utils/t_macros.h"

#include "modular_project/ModularSmartcontractProject.h"

using namespace codablecash;


TEST_GROUP(TestModularProjectGroup) {
	TEST_SETUP() {
		env->setup();
	}
	TEST_TEARDOWN() {
		env->teardown();
	}
};

TEST(TestModularProjectGroup, case01) {
	File projectFolder = this->env->testCaseDir();

	File* baseDir = projectFolder.get(L"projectbase");

	ModularSmartcontractProject project(baseDir);

	// FIXME TestModularProjectGroup
}

