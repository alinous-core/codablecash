/*
 * ModularSmartcontractInstance.cpp
 *
 *  Created on: Sep 22, 2025
 *      Author: iizuka
 */

#include "smartcontract_instance/ModularSmartcontractInstance.h"
#include "smartcontract_instance/ExecutableModuleInstance.h"
#include "smartcontract_instance/LibraryExectableModuleInstance.h"

#include "modular_project/ModularProjectConfig.h"

#include "base/StackRelease.h"
#include "base/UnicodeString.h"


namespace codablecash {

ModularSmartcontractInstance::ModularSmartcontractInstance() {
	this->config = nullptr;
	this->execModule = nullptr;
	this->libraries = new HashMap<UnicodeString, LibraryExectableModuleInstance>();
	this->libArray = new ArrayList<LibraryExectableModuleInstance>();
}

ModularSmartcontractInstance::~ModularSmartcontractInstance() {
	delete this->config;
	delete this->execModule;

	{
		Iterator<UnicodeString>* it = this->libraries->keySet()->iterator(); __STP(it);
		while(it->hasNext()){
			const UnicodeString* key = it->next();
			LibraryExectableModuleInstance* inst = this->libraries->get(key);

			delete inst;
		}
	}
	delete this->libraries;

	delete this->libArray;
}

void ModularSmartcontractInstance::setModularProjectConfig(const ModularProjectConfig *config) {
	delete this->config;
	this->config = config->copy();
}

void ModularSmartcontractInstance::setExecutableModuleInstance(ExecutableModuleInstance *inst) {
	this->execModule = inst;
}

void ModularSmartcontractInstance::addLibraryModuleInstance(const UnicodeString *name, LibraryExectableModuleInstance *inst) {
	this->libraries->put(name, inst);
	this->libArray->addElement(inst);
}

void ModularSmartcontractInstance::loadCompilantUnits(const File *projectBaseDir) {
	this->execModule->loadCompilantUnits(projectBaseDir);

	int maxLoop = this->libArray->size();
	for(int i = 0; i != maxLoop; ++i){
		LibraryExectableModuleInstance* inst = this->libArray->get(i);
		inst->loadCompilantUnits(projectBaseDir);
	}

}

bool ModularSmartcontractInstance::libraryExists(const UnicodeString *name) const noexcept {
	LibraryExectableModuleInstance* value = this->libraries->get(name);
	return value != nullptr;
}

} /* namespace codablecash */
