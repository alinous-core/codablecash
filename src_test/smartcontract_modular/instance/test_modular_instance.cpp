/*
 * test_modular_instance.cpp
 *
 *  Created on: Sep 19, 2025
 *      Author: iizuka
 */
#include "test_utils/t_macros.h"

#include "../utils/ModularProjectUtils.h"

#include "modular_project/ModularSmartcontractProject.h"

#include "smartcontract_instance/ModularSmartcontractInstance.h"

using namespace codablecash;


TEST_GROUP(TestModularInstanceGroup) {
	TEST_SETUP() {
		env->setup();
	}
	TEST_TEARDOWN() {
		env->teardown();
	}
};


TEST(TestModularInstanceGroup, case01){
	const File* projectFolder = this->env->getProjectRoot();

	ModularProjectUtils utils(L"src_test/smartcontract_modular/instance/resources/case01/project01/", projectFolder);
	const File* baseDir = utils.getFolder();

	ModularSmartcontractProject project(baseDir);
	project.loadProject();


	ModularSmartcontractInstance* inst = project.toInstance(); __STP(inst);
	inst->loadCompilantUnits(baseDir);

	bool res = inst->hasCompileError();
	CHECK(res == false);

	res = inst->analyze();
	CHECK(res == false);

	inst->setMainInstance();
	res = inst->createMainInstance();
	CHECK(res == false);

	inst->resetRootReference();

	inst->setMainInstance();
	res = inst->createMainInstance();
	CHECK(res == false);


	// FIXME TestModularInstanceGroup
}

TEST(TestModularInstanceGroup, case02){
	const File* projectFolder = this->env->getProjectRoot();

	ModularProjectUtils utils(L"src_test/smartcontract_modular/instance/resources/case02/project01/", projectFolder);
	const File* baseDir = utils.getFolder();

	ModularSmartcontractProject project(baseDir);
	project.loadProject();

	ModularSmartcontractInstance* inst = project.toInstance(); __STP(inst);
	inst->loadCompilantUnits(baseDir);

	bool res = inst->hasCompileError();
	CHECK(res == false);

	res = inst->analyze();
	CHECK(res == false);

	inst->setMainInstance();
	res = inst->createMainInstance();
	CHECK(res == false);
}


