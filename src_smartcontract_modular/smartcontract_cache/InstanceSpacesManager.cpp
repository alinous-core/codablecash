/*
 * InstanceSpacesManager.cpp
 *
 *  Created on: Nov 30, 2025
 *      Author: iizuka
 */

#include "smartcontract_cache/InstanceSpacesManager.h"
#include "smartcontract_cache/MolularSmartcontractCacheManager.h"

#include "smartcontract_executor/SmartcontractExecContextRegistory.h"

#include "smartcontract_executor_index/InstanceIdIndex.h"

#include "modular_project_registory/ModularSmartcontractProjectRegistory.h"
#include "modular_project_registory/ProjectIdKey.h"
#include "modular_project_registory/SmartcontractProjectData.h"

#include "smartcontract_cache/InstanceSpace.h"

#include "base/StackRelease.h"


namespace codablecash {

InstanceSpacesManager::InstanceSpacesManager(const File* baseDir, SmartcontractExecContextRegistory* execContextReg
		, InstanceIdIndex* instanceIdIndex, ModularSmartcontractProjectRegistory* projectReg) {
	this->execContextReg = execContextReg;
	this->instanceIdIndex = instanceIdIndex;
	this->projectReg = projectReg;

	this->cache = new MolularSmartcontractCacheManager(1024);
}

InstanceSpacesManager::~InstanceSpacesManager() {
	this->execContextReg = nullptr;
	this->instanceIdIndex = nullptr;
	this->projectReg = nullptr;

	delete this->cache;
}

InstanceSpace* InstanceSpacesManager::createInstance(const SmartcontractInstanceAddress *instAddress, const SmartcontractProjectId *projectId) {
	ProjectIdKey key;
	key.setProjectId(projectId);

	SmartcontractProjectData* data = this->projectReg->findProjectById(&key); __STP(data);
	if(data == nullptr){ // FIXME exception
		return nullptr;
	}

	ModularSmartcontractInstance* instance = data->toSmartcontractInstance();
	InstanceSpace* space = new InstanceSpace(instAddress, instance); __STP(space);

	this->cache->registerInstance(space);

	return __STP_MV(space);
}

} /* namespace codablecash */
