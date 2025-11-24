/*
 * LibraryExectableModuleInstance.cpp
 *
 *  Created on: Nov 11, 2025
 *      Author: iizuka
 */

#include "smartcontract_instance/LibraryExectableModuleInstance.h"
#include "smartcontract_instance/ModuleInstanceClassLoader.h"

#include "base/UnicodeString.h"

#include "vm/VirtualMachine.h"


namespace codablecash {

LibraryExectableModuleInstance::LibraryExectableModuleInstance()
		: AbstractExecutableModuleInstance() {
	this->libraryName = nullptr;
	this->exportClasses = new ArrayList<UnicodeString>();
}

LibraryExectableModuleInstance::~LibraryExectableModuleInstance() {
	delete this->libraryName;

	this->exportClasses->deleteElements();
	delete this->exportClasses;
}

void LibraryExectableModuleInstance::setLibraryName(const UnicodeString *str) noexcept {
	delete this->libraryName;
	this->libraryName = new UnicodeString(str);
}

void LibraryExectableModuleInstance::addExportClass(const UnicodeString *clazz) noexcept {
	this->exportClasses->addElement(new UnicodeString(clazz));
}

void LibraryExectableModuleInstance::loadCompilantUnits(const File *projectBaseDir) {
	resetContract();
	parseSourceFolders(projectBaseDir);
}

void LibraryExectableModuleInstance::loadExportClasses(ModuleInstanceClassLoader *loader) {
	SmartContract* contract = this->vm->getSmartContract();

	int maxLoop = this->exportClasses->size();
	for(int i = 0; i != maxLoop; ++i){
		UnicodeString* fqn = this->exportClasses->get(i);

		loader->loadClass(fqn);
	}
}

} /* namespace codablecash */
