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
#include "base/Exception.h"
#include "base/exceptions.h"

#include "bc/ExceptionThrower.h"

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

LibraryExectableModuleInstance* ModularSmartcontractInstance::getLibraryModuleInstance(const UnicodeString *modName) const noexcept {
	LibraryExectableModuleInstance* value = this->libraries->get(modName);
	return value;
}

bool ModularSmartcontractInstance::hasCompileError() const noexcept {
	bool hasError = this->execModule->hasCompileError();

	int maxLoop = this->libArray->size();
	for(int i = 0; i != maxLoop; ++i){
		LibraryExectableModuleInstance* inst = this->libArray->get(i);
		hasError |= inst->hasCompileError();
	}

	return hasError;
}

bool ModularSmartcontractInstance::analyze() {
	bool hasError = false;

	try{
		initBeforeAnalyze();
		preAnalyze();
		preAnalyzeGenerics();
		loadDependency();
		preAnalyzeDependency();

		analyzeType();
		analyzeTypeDependency();
		analyzeMetadata();
		analyzeFinal();
		analyzeDependency();
	}
	catch(CompilantUnitAnalyzeException* e){
		delete e;
		hasError = true;
	}

	return hasError;
}

void ModularSmartcontractInstance::initBeforeAnalyze() {
	bool hasError = this->execModule->initBeforeAnalyze();

	int maxLoop = this->libArray->size();
	for(int i = 0; i != maxLoop; ++i){
		LibraryExectableModuleInstance* inst = this->libArray->get(i);
		hasError |= inst->initBeforeAnalyze();
	}

	ExceptionThrower<CompilantUnitAnalyzeException>::throwExceptionIfCondition(hasError, L"initBeforeAnalyze() failed.", __FILE__, __LINE__);
}

void ModularSmartcontractInstance::preAnalyze() {
	bool hasError = this->execModule->preAnalyze();

	int maxLoop = this->libArray->size();
	for(int i = 0; i != maxLoop; ++i){
		LibraryExectableModuleInstance* inst = this->libArray->get(i);
		hasError |= inst->preAnalyze();
	}

	ExceptionThrower<CompilantUnitAnalyzeException>::throwExceptionIfCondition(hasError, L"preAnalyze() failed.", __FILE__, __LINE__);
}

void ModularSmartcontractInstance::preAnalyzeGenerics() {
	bool hasError = this->execModule->preAnalyzeGenerics();

	int maxLoop = this->libArray->size();
	for(int i = 0; i != maxLoop; ++i){
		LibraryExectableModuleInstance* inst = this->libArray->get(i);
		hasError |= inst->preAnalyzeGenerics();
	}

	ExceptionThrower<CompilantUnitAnalyzeException>::throwExceptionIfCondition(hasError, L"preAnalyzeGenerics() failed.", __FILE__, __LINE__);
}

void ModularSmartcontractInstance::analyzeType() {
	bool hasError = this->execModule->analyzeType();

	int maxLoop = this->libArray->size();
	for(int i = 0; i != maxLoop; ++i){
		LibraryExectableModuleInstance* inst = this->libArray->get(i);
		hasError |= inst->analyzeType();
	}

	ExceptionThrower<CompilantUnitAnalyzeException>::throwExceptionIfCondition(hasError, L"analyzeType() failed.", __FILE__, __LINE__);
}

void ModularSmartcontractInstance::analyzeMetadata() {
	bool hasError = this->execModule->analyzeMetadata();

	int maxLoop = this->libArray->size();
	for(int i = 0; i != maxLoop; ++i){
		LibraryExectableModuleInstance* inst = this->libArray->get(i);
		hasError |= inst->analyzeMetadata();
	}

	ExceptionThrower<CompilantUnitAnalyzeException>::throwExceptionIfCondition(hasError, L"analyzeMetadata() failed.", __FILE__, __LINE__);
}

void ModularSmartcontractInstance::analyzeFinal() {
	bool hasError = this->execModule->analyzeFinal();

	int maxLoop = this->libArray->size();
	for(int i = 0; i != maxLoop; ++i){
		LibraryExectableModuleInstance* inst = this->libArray->get(i);
		hasError |= inst->analyzeFinal();
	}

	ExceptionThrower<CompilantUnitAnalyzeException>::throwExceptionIfCondition(hasError, L"analyzeFinal() failed.", __FILE__, __LINE__);
}

void ModularSmartcontractInstance::loadDependency() {
	bool hasError = false;

	int maxLoop = this->libArray->size();
	for(int i = 0; i != maxLoop; ++i){
		LibraryExectableModuleInstance* inst = this->libArray->get(i);
		hasError |= inst->loadDependency(this);
	}

	hasError = this->execModule->loadDependency(this);

	ExceptionThrower<CompilantUnitAnalyzeException>::throwExceptionIfCondition(hasError, L"loadDependency() failed.", __FILE__, __LINE__);
}

void ModularSmartcontractInstance::preAnalyzeDependency() {
	bool hasError = false;

	int maxLoop = this->libArray->size();
	for(int i = 0; i != maxLoop; ++i){
		LibraryExectableModuleInstance* inst = this->libArray->get(i);
		hasError |= inst->preAnalyzeDependency();
	}

	hasError = this->execModule->preAnalyzeDependency();

	ExceptionThrower<CompilantUnitAnalyzeException>::throwExceptionIfCondition(hasError, L"preAnalyzeDependency() failed.", __FILE__, __LINE__);
}

void ModularSmartcontractInstance::analyzeTypeDependency() {
	bool hasError = false;

	int maxLoop = this->libArray->size();
	for(int i = 0; i != maxLoop; ++i){
		LibraryExectableModuleInstance* inst = this->libArray->get(i);
		hasError |= inst->analyzeTypeDependency();
	}

	hasError = this->execModule->analyzeTypeDependency();

	ExceptionThrower<CompilantUnitAnalyzeException>::throwExceptionIfCondition(hasError, L"preAnalyzeDependency() failed.", __FILE__, __LINE__);
}

void ModularSmartcontractInstance::analyzeDependency() {
	bool hasError = this->execModule->analyzeDependency();

	int maxLoop = this->libArray->size();
	for(int i = 0; i != maxLoop; ++i){
		LibraryExectableModuleInstance* inst = this->libArray->get(i);
		hasError |= inst->analyzeDependency();
	}

	ExceptionThrower<CompilantUnitAnalyzeException>::throwExceptionIfCondition(hasError, L"analyzeDependency() failed.", __FILE__, __LINE__);
}

void ModularSmartcontractInstance::setMainInstance() {
	this->execModule->setMainInstance();

	int maxLoop = this->libArray->size();
	for(int i = 0; i != maxLoop; ++i){
		LibraryExectableModuleInstance* inst = this->libArray->get(i);
		inst->setMainInstance();
	}
}

bool ModularSmartcontractInstance::createMainInstance() {
	bool hasError = !this->execModule->createMainInstance();

	int maxLoop = this->libArray->size();
	for(int i = 0; i != maxLoop; ++i){
		LibraryExectableModuleInstance* inst = this->libArray->get(i);
		hasError |= (!inst->createMainInstance());
	}

	return hasError;
}

bool ModularSmartcontractInstance::interpretInitializer() {
	bool hasError = !this->execModule->interpretInitializer();

	int maxLoop = this->libArray->size();
	for(int i = 0; i != maxLoop; ++i){
		LibraryExectableModuleInstance* inst = this->libArray->get(i);
		hasError |= (!inst->interpretInitializer());
	}

	return hasError;
}

void ModularSmartcontractInstance::resetRootReference() {
	this->execModule->resetRootReference();

	int maxLoop = this->libArray->size();
	for(int i = 0; i != maxLoop; ++i){
		LibraryExectableModuleInstance* inst = this->libArray->get(i);
		inst->resetRootReference();
	}
}

} /* namespace codablecash */
