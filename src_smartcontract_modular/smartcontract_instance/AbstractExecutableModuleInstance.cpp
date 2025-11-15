/*
 * AbstractExecutableModule.cpp
 *
 *  Created on: Nov 11, 2025
 *      Author: iizuka
 */
#include "smartcontract_instance/AbstractExecutableModuleInstance.h"

#include "base/UnicodeString.h"
#include "base/StackRelease.h"

#include "bc/SoftwareVersion.h"

#include "modular_project/ModularInstanceConfig.h"
#include "modular_project/DependencyConfig.h"

#include "base_io/ByteBuffer.h"


namespace codablecash {

AbstractExecutableModuleInstance::AbstractExecutableModuleInstance() {
	this->name = nullptr;
	this->projectRelativePath = nullptr;

	this->sourceFolders = new ArrayList<UnicodeString>();
	this->version = nullptr;
	this->instanceConfig = nullptr;
	this->dependencyConfig = nullptr;
}

AbstractExecutableModuleInstance::~AbstractExecutableModuleInstance() {
	delete this->name;
	delete this->projectRelativePath;

	this->sourceFolders->deleteElements();
	delete this->sourceFolders;

	delete this->version;
	delete this->instanceConfig;
	delete this->dependencyConfig;
}

void AbstractExecutableModuleInstance::setName(const UnicodeString *name) noexcept {
	delete this->name;
	this->name = new UnicodeString(name);
}

void AbstractExecutableModuleInstance::setProjectRelativePath(const UnicodeString *path) noexcept {
	delete this->projectRelativePath;
	this->projectRelativePath = new UnicodeString(path);
}

void AbstractExecutableModuleInstance::addSourceFolders(const UnicodeString *folder) {
	this->sourceFolders->addElement(new UnicodeString(folder));
}

void AbstractExecutableModuleInstance::setSoftwareVersion(const SoftwareVersion *v) {
	delete this->version;
	this->version = new SoftwareVersion(*v);
}

void AbstractExecutableModuleInstance::setInstanceConfig(const ModularInstanceConfig *config) {
	delete this->instanceConfig;

	int cap = config->binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(cap, true); __STP(buff);
	config->toBinary(buff);

	buff->position(0);
	this->instanceConfig = ModularInstanceConfig::createFromBinary(buff);
}

void AbstractExecutableModuleInstance::setDependencyConfig(const DependencyConfig *config) {
	delete this->dependencyConfig;
	this->dependencyConfig = new DependencyConfig(*config);
}

} /* namespace alinous */
