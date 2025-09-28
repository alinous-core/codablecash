/*
 * ModularSmartcontractProject.cpp
 *
 *  Created on: Sep 19, 2025
 *      Author: iizuka
 */

#include "modular_project/ModularSmartcontractProject.h"
#include "modular_project/ExecutableSmartcontractModule.h"
#include "modular_project/LibrarySmartcontractModule.h"

#include "base_io/File.h"


namespace codablecash {

ModularSmartcontractProject::ModularSmartcontractProject(const File* baseDir) {
	this->baseDir = new File(*baseDir);

	this->executableModule = nullptr;
	this->libModules = new ArrayList<LibrarySmartcontractModule>();
}

ModularSmartcontractProject::~ModularSmartcontractProject() {
	delete this->baseDir;

	delete this->executableModule;

	this->libModules->deleteElements();
	delete this->libModules;
}

} /* namespace codablecash */
