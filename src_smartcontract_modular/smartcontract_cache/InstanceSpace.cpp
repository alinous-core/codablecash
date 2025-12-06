/*
 * InstanceSpace.cpp
 *
 *  Created on: Nov 28, 2025
 *      Author: iizuka
 */

#include "smartcontract_cache/InstanceSpace.h"

#include "smartcontract_instance/ModularSmartcontractInstance.h"

#include "smartcontract_executor/SmartcontractExecResult.h"

#include "base_thread/SysMutex.h"
#include "base_thread/StackUnlocker.h"

#include "base/UnicodeString.h"
#include "base/StackRelease.h"


namespace codablecash {

InstanceSpace::InstanceSpace(const SmartcontractInstanceAddress *instAddress, ModularSmartcontractInstance* instance) {
	this->instance = instance;
	this->instance->setSmartcontractInstanceAddress(instAddress);

	this->mutex = new SysMutex();
	this->ref = 0;
}

InstanceSpace::~InstanceSpace() {
	delete this->instance;
	delete this->mutex;
}

bool InstanceSpace::isDeletable() const noexcept {
	StackUnlocker __lock(this->mutex, __FILE__, __LINE__);
	return this->ref == 0;
}

void InstanceSpace::incRef() {
	StackUnlocker __lock(this->mutex, __FILE__, __LINE__);
	this->ref++;
}

void InstanceSpace::decRef() {
	StackUnlocker __lock(this->mutex, __FILE__, __LINE__);
	this->ref--;
}

void InstanceSpace::beforeCacheOut() {
	this->instance->cleanDbRoot();
}

const SmartcontractInstanceAddress* InstanceSpace::getSmartContractInstanceAddress() const noexcept {
	return this->instance->getSmartContractInstanceAddress();
}

bool InstanceSpace::analyze() {
	bool hasError = this->instance->analyze();

	this->instance->setMainInstance();

	// check direct Interface
	if(!hasError){
		hasError |= this->instance->checkDirectAccess();
	}

	// generate connector class
	if(!hasError){
		hasError |= this->instance->generateInterModularCommunicationClasses();
	}

	return hasError;
}

void InstanceSpace::setMainInstance() {
	this->instance->setMainInstance();
}

bool InstanceSpace::createMainInstance() {
	return this->instance->createMainInstance();
}

bool InstanceSpace::interpretInitializer() {
	return this->instance->interpretInitializer();
}

void InstanceSpace::resetRootReference() {
	this->instance->resetRootReference();
}

void InstanceSpace::setDatabaseDir(const File *baseDir) {
	this->instance->setDatabaseDir(baseDir);
}

void InstanceSpace::createDatabase() {
	this->instance->createDatabase();
}

void InstanceSpace::loadDatabase() {
	this->instance->loadDatabase();
}

void InstanceSpace::cleanDbRoot() {
	this->instance->cleanDbRoot();
}

SmartcontractExecResult* InstanceSpace::invokeMainObjectMethod(UnicodeString *moduleName, UnicodeString *methodName, ArrayList<AbstractFunctionExtArguments>* args) {
	SmartcontractExecResult* result = this->instance->invokeMainObjectMethod(moduleName, methodName, args); __STP(result);

	return __STP_MV(result);
}

} /* namespace codablecash */
