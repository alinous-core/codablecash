/*
 * ModularSmartcontractExecutor.cpp
 *
 *  Created on: Sep 22, 2025
 *      Author: iizuka
 */

#include "smartcontract_executor/ModularSmartcontractExecutor.h"
#include "smartcontract_executor/SmartcontractExecContextRegistory.h"
#include "smartcontract_executor_index/InstanceIdIndex.h"

#include "modular_project/ModularSmartcontractProject.h"

#include "modular_project_registory/ModularSmartcontractProjectRegistory.h"

#include "smartcontract_instance/ModularSmartcontractInstance.h"

#include "base/UnicodeString.h"
#include "base/StackRelease.h"

#include "base_io/File.h"


namespace codablecash {

ModularSmartcontractExecutor::ModularSmartcontractExecutor(const File* base) {
	this->baseDir = base->get(DIR_NAME);
	this->projectRegistory = new ModularSmartcontractProjectRegistory(this->baseDir);
	this->execContextRegistory = new SmartcontractExecContextRegistory(this->baseDir);
	this->instanceIndex = new InstanceIdIndex(this->baseDir);
}

ModularSmartcontractExecutor::~ModularSmartcontractExecutor() {
	delete this->projectRegistory;
	delete this->execContextRegistory;
	delete this->instanceIndex;
	delete this->baseDir;
}

void ModularSmartcontractExecutor::registerModularSmartcontractProject(ModularSmartcontractProject *project) {
	project->loadProject();

	ModularSmartcontractInstance* inst = project->toInstance(); __STP(inst);
	inst->loadCompilantUnits(this->baseDir);


}

} /* namespace codablecash */
