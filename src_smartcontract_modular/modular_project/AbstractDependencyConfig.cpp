/*
 * AbstractDependencyConfig.cpp
 *
 *  Created on: Oct 8, 2025
 *      Author: iizuka
 */

#include "modular_project/AbstractDependencyConfig.h"

#include "modular_project/ModularSmartcontractVersion.h"

#include "base/UnicodeString.h"



namespace codablecash {

AbstractDependencyConfig::AbstractDependencyConfig() {
	this->moduleName = nullptr;
	this->version = nullptr;
}

AbstractDependencyConfig::~AbstractDependencyConfig() {
	delete this->moduleName;
	delete this->version;
}

} /* namespace codablecash */
