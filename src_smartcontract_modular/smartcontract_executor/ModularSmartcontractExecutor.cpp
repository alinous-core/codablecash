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
#include "modular_project_registory/SmartcontractProjectData.h"
#include "modular_project_registory/ProjectIdKey.h"

#include "smartcontract_instance/ModularSmartcontractInstance.h"

#include "smartcontract_cache/InstanceSpacesManager.h"
#include "smartcontract_cache/InstanceSpace.h"

#include "base/UnicodeString.h"
#include "base/StackRelease.h"

#include "base_io/File.h"

#include "bc/ExceptionThrower.h"

#include "smartcontract_cache/ModuleSetupException.h"
namespace codablecash {

ModularSmartcontractExecutor::ModularSmartcontractExecutor(const File* base) {
	this->baseDir = base->get(DIR_NAME);
	this->projectRegistory = new ModularSmartcontractProjectRegistory(this->baseDir);
	this->execContextRegistory = new SmartcontractExecContextRegistory(this->baseDir);
	this->instanceIndex = new InstanceIdIndex(this->baseDir);

	this->instanceSpace = new InstanceSpacesManager(this->baseDir, this->execContextRegistory, this->instanceIndex, this->projectRegistory);
}

ModularSmartcontractExecutor::~ModularSmartcontractExecutor() {
	delete this->projectRegistory;
	delete this->execContextRegistory;
	delete this->instanceIndex;
	delete this->baseDir;
	delete this->instanceSpace;
}

void ModularSmartcontractExecutor::createExecutor() {
	this->projectRegistory->createBlankDatabase();
	this->execContextRegistory->createBlankDatabase();
	this->instanceIndex->createBlankDatabase();
}

void ModularSmartcontractExecutor::open() {
	this->projectRegistory->open();
	this->execContextRegistory->open();
	this->instanceIndex->open();
}

void ModularSmartcontractExecutor::close() noexcept {
	this->projectRegistory->close();
	this->execContextRegistory->close();
	this->instanceIndex->close();
}

void ModularSmartcontractExecutor::registerModularSmartcontractProject(ModularSmartcontractProject *project) {
	project->loadProject();

	ModularSmartcontractInstance* inst = project->toInstance(); __STP(inst);
	inst->loadCompilantUnits(project->getProjectBaseDir());
	bool res = inst->hasCompileError();

	SmartcontractProjectData* data = inst->createData(); __STP(data);

	const ProjectIdKey* key = data->getKey();
	this->projectRegistory->put(key, data);
}

SmartcontractProjectData* ModularSmartcontractExecutor::getProject(const SmartcontractProjectId *projectId) {
	ProjectIdKey key;
	key.setProjectId(projectId);

	SmartcontractProjectData* data = this->projectRegistory->findProjectById(&key);
	return data;
}

void ModularSmartcontractExecutor::createInstance(const SmartcontractInstanceAddress *instAddress, const SmartcontractProjectId *projectId) {
	InstanceSpace* space = this->instanceSpace->createInstance(instAddress, projectId); __STP(space);

	// language
	bool hasError = space->analyze();
	ExceptionThrower<ModuleSetupException>::throwExceptionIfCondition(hasError == true, L"Analysis error.", __FILE__, __LINE__);

	space->setMainInstance();
	space->createMainInstance();
	hasError = space->interpretInitializer();
	ExceptionThrower<ModuleSetupException>::throwExceptionIfCondition(hasError == true, L"Main object initialize error.", __FILE__, __LINE__);

	// database
	File* instanceRootDir = this->baseDir->get(INSTANCES_DIR_NAME); __STP(instanceRootDir);
	space->setDatabaseDir(instanceRootDir);
	space->createDatabase();

	this->instanceSpace->registerCache(__STP_MV(space));
}

InstanceSpace* ModularSmartcontractExecutor::loadFromCache(const SmartcontractInstanceAddress *instAddress) {
	InstanceSpace* space = this->instanceSpace->loadFromCache(instAddress);
	return space;
}

} /* namespace codablecash */
