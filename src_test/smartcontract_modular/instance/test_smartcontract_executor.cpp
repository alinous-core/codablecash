/*
 * test_smartcontract_executor.cpp
 *
 *  Created on: Nov 28, 2025
 *      Author: iizuka
 */
#include "test_utils/t_macros.h"

#include "test_utils/t_macros.h"

#include "../utils/ModularProjectUtils.h"

#include "modular_project/ModularSmartcontractProject.h"

#include "smartcontract_instance/ModularSmartcontractInstance.h"

#include "modular_project_registory/SmartcontractProjectData.h"
#include "modular_project_registory/ModularSmartcontractProjectRegistory.h"

#include "trx/session/base/CdbDatabaseSessionId.h"

#include "smartcontract_executor/ModularSmartcontractExecutor.h"


using namespace codablecash;

TEST_GROUP(TestSmartcontractExecutorGroup) {
	TEST_SETUP() {
		env->setup();
	}
	TEST_TEARDOWN() {
		env->teardown();
	}
};

TEST(TestSmartcontractExecutorGroup, case01){
	const File* projectFolder = this->env->getProjectRoot();
	ModularProjectUtils utils(L"src_test/smartcontract_modular/project/resources/reg_case01/project01/", projectFolder);
	const File* baseDir = utils.getFolder();

	ModularSmartcontractProject project(baseDir);
	project.loadProject();

	// Executor
	File projectTestFolder = this->env->testCaseDir();
	_ST(File, baseTestDir, projectTestFolder.get(L"exec"))

	ModularSmartcontractExecutor executopr(baseTestDir);
}
